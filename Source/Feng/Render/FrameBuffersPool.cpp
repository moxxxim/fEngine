#include <Feng/Render/FrameBuffersPool.h>

#include <Feng/App/Globals.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    FrameBuffersPool::~FrameBuffersPool()
    {
        for(const FrameBuffer& buffer : buffers)
        {
            DeleteBuffer(buffer);
        }
    }
    
    FrameBuffer FrameBuffersPool::Pop(const FrameBuffer::Settings& settings)
    {
        auto isSuitable = [&settings](const FrameBuffer& buffer)
        {
            return buffer.settings == settings;
        };
        
        auto it = std::find_if(buffers.begin(), buffers.end(), isSuitable);
        if(it != buffers.end())
        {
            FrameBuffer outBuffer = *it;
            std::swap(*it, buffers.back());
            buffers.pop_back();
            return outBuffer;
        }
        
        return CreateBuffer(settings);
    }

    FrameBuffer FrameBuffersPool::CreateBuffer(const FrameBuffer::Settings& settings)
    {
        GLsizei samplesCount = 4;
        GLenum target = settings.multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        GLuint colorBuffer = 0;
        if(settings.color == FrameBuffer::eAttachementState::Texture)
        {
            glGenTextures(1, &colorBuffer);
            glBindTexture(target, colorBuffer);

            if(target == GL_TEXTURE_2D)
            {
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexImage2D(
                             GL_TEXTURE_2D,
                             0,
                             GL_RGB,
                             settings.size.width,
                             settings.size.height,
                             0,
                             GL_RGB,
                             GL_UNSIGNED_BYTE,
                             nullptr);
            }
            else if (target == GL_TEXTURE_2D_MULTISAMPLE)
            {
                glTexImage2DMultisample(
                                        GL_TEXTURE_2D_MULTISAMPLE,
                                        samplesCount,
                                        GL_RGB,
                                        settings.size.width,
                                        settings.size.height,
                                        GL_TRUE);
            }

            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, colorBuffer, 0);
        }
        else if (settings.color == FrameBuffer::eAttachementState::Buffer)
        {
            Debug::LogError("Color buffer as RenderBuffer not implemented yet.");
        }
        else
        {
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        }
        
        Print_Errors_OpengGL();

        GLuint depth = 0;
        if (settings.depth == FrameBuffer::eAttachementState::Texture)
        {
            GLenum texType = GL_TEXTURE_2D;
            if(settings.isDepthCubemap)
            {
                texType = GL_TEXTURE_CUBE_MAP;
            }
            else if (settings.multisample)
            {
                texType = GL_MULTISAMPLE;
            }
            
            glGenTextures(1, &depth);
            glBindTexture(texType, depth);

            if((texType == GL_TEXTURE_2D) || (texType == GL_TEXTURE_CUBE_MAP))
            {
                glTexParameteri(texType, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                glTexParameteri(texType, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                glTexParameteri(texType, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(texType, GL_TEXTURE_WRAP_T, GL_REPEAT);
                if(texType == GL_TEXTURE_CUBE_MAP)
                {
                    glTexParameteri(texType, GL_TEXTURE_WRAP_R, GL_REPEAT);
                }

                if(texType == GL_TEXTURE_2D)
                {
                    glTexImage2D(
                                 texType,
                                 0,
                                 GL_DEPTH_COMPONENT,
                                 settings.size.width,
                                 settings.size.height,
                                 0,
                                 GL_DEPTH_COMPONENT,
                                 GL_FLOAT,
                                 nullptr);
                }
                else
                {
                    for (int32_t i = 0; i < 6; ++i)
                    {
                        const GLenum face = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
                        glTexImage2D(
                                     face,
                                     0,
                                     GL_DEPTH_COMPONENT,
                                     settings.size.width,
                                     settings.size.height,
                                     0,
                                     GL_DEPTH_COMPONENT,
                                     GL_FLOAT,
                                     nullptr);
                    }
                }
            }
            else if (texType == GL_TEXTURE_2D_MULTISAMPLE)
            {
                glTexImage2DMultisample(
                                        texType,
                                        samplesCount,
                                        GL_RGB,
                                        settings.size.width,
                                        settings.size.height,
                                        GL_TRUE);
            }
            
            Print_Errors_OpengGL();

            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depth, 0);
            Print_Errors_OpengGL();
        }
        else if (settings.depth == FrameBuffer::eAttachementState::Buffer)
        {
            glGenRenderbuffers(1, &depth);
            glBindRenderbuffer(GL_RENDERBUFFER, depth);
            
            GLenum format = settings.combinedDepthStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT;
            if(target == GL_TEXTURE_2D)
            {
                glRenderbufferStorage(GL_RENDERBUFFER, format, settings.size.width, settings.size.height);
            }
            else if (target == GL_TEXTURE_2D_MULTISAMPLE)
            {
                glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                                 samplesCount,
                                                 format,
                                                 settings.size.width,
                                                 settings.size.height);
            }

            GLenum attachment = settings.combinedDepthStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
            glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, depth);
        }
        
        Print_Errors_OpengGL();
        
        GLuint stencil = 0;
        if ((settings.stencil != FrameBuffer::eAttachementState::None) && !settings.combinedDepthStencil)
        {
            Debug::LogError("Separate stencil buffer not implemented yet.");
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            Debug::LogError("Cannot create frame buffer.");
        }
        
        Print_Errors_OpengGL();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        FrameBuffer frameBuffer;
        frameBuffer.frame = fbo;
        frameBuffer.color = colorBuffer;
        frameBuffer.depth = depth;
        frameBuffer.stencil = stencil;
        frameBuffer.settings = settings;

        return frameBuffer;
    }
    
    void FrameBuffersPool::Push(const FrameBuffer& buffer)
    {
        buffers.push_back(buffer);
    }
    
    void FrameBuffersPool::DeleteBuffer(const FrameBuffer& buffer)
    {
        glDeleteFramebuffers(1, &buffer.frame);
        glDeleteTextures(1, &buffer.color);
        glDeleteRenderbuffers(1, &buffer.depth);
        glDeleteRenderbuffers(1, &buffer.stencil);
    }
}
