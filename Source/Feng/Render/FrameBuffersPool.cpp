#include <Feng/Render/FrameBuffersPool.h>

#include <Feng/App/Globals.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    namespace SFrameBuffersPool
    {
        constexpr GLsizei samplesCount = 4;
        
        GLuint BindColorBuffer(const FrameBuffer::Settings& settings)
        {
            GLuint colorBuffer = 0;
            
            GLenum target = settings.multisample
                ? GL_TEXTURE_2D_MULTISAMPLE
                : FrameBuffersPool::GetBindTarget(settings.color);
            if(settings.color == FrameBuffer::eAttachement::Texture2d)
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
                                 target,
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
                                            target,
                                            samplesCount,
                                            GL_RGB,
                                            settings.size.width,
                                            settings.size.height,
                                            GL_TRUE);
                }

                glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, colorBuffer, 0);
            }
            else if (settings.color == FrameBuffer::eAttachement::Buffer)
            {
                Debug::LogError("Color buffer as RenderBuffer not implemented yet.");
            }
            else if (settings.color == FrameBuffer::eAttachement::Texture2dArray)
            {
                Debug::LogError("Color buffer as Texture 2D Array not implemented yet.");
            }
            else
            {
                glDrawBuffer(GL_NONE);
                glReadBuffer(GL_NONE);
            }
            
            return colorBuffer;
        }
        
        GLuint BindDepthBuffer(const FrameBuffer::Settings& settings)
        {
            GLuint depth = 0;
            GLenum target = settings.multisample
                ? GL_TEXTURE_2D_MULTISAMPLE
                : FrameBuffersPool::GetBindTarget(settings.depth);
            
            if (settings.depth != FrameBuffer::eAttachement::Buffer)
            {
                glGenTextures(1, &depth);
                glBindTexture(target, depth);

                if(target != GL_TEXTURE_2D_MULTISAMPLE)
                {
                    glTexParameteri(target, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
                    glTexParameteri(target, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
                    glTexParameteri(target, GL_TEXTURE_WRAP_S, GL_REPEAT);
                    glTexParameteri(target, GL_TEXTURE_WRAP_T, GL_REPEAT);
                    if(target == GL_TEXTURE_CUBE_MAP)
                    {
                        glTexParameteri(target, GL_TEXTURE_WRAP_R, GL_REPEAT);
                    }

                    if(target == GL_TEXTURE_2D)
                    {
                        glTexImage2D(
                                     target,
                                     0,
                                     GL_DEPTH_COMPONENT,
                                     settings.size.width,
                                     settings.size.height,
                                     0,
                                     GL_DEPTH_COMPONENT,
                                     GL_FLOAT,
                                     nullptr);
                    }
                    else if (target == GL_TEXTURE_2D_ARRAY)
                    {
                        glTexImage3D(
                            target,
                            0,
                            GL_DEPTH_COMPONENT32F,
                            settings.size.width,
                            settings.size.height,
                            settings.depth2dArraySize,
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
                else
                {
                    glTexImage2DMultisample(
                                            target,
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
            else if (settings.depth == FrameBuffer::eAttachement::Buffer)
            {
                glGenRenderbuffers(1, &depth);
                glBindRenderbuffer(GL_RENDERBUFFER, depth);
                
                GLenum format = settings.combinedDepthStencil ? GL_DEPTH24_STENCIL8 : GL_DEPTH_COMPONENT;
                if(settings.multisample)
                {
                    glRenderbufferStorageMultisample(GL_RENDERBUFFER,
                                                     samplesCount,
                                                     format,
                                                     settings.size.width,
                                                     settings.size.height);
                }
                else
                {
                    glRenderbufferStorage(GL_RENDERBUFFER, format, settings.size.width, settings.size.height);
                }

                GLenum attachment = settings.combinedDepthStencil ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT;
                glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, depth);
                Print_Errors_OpengGL();
            }
            
            return depth;
        }
        
        GLuint BindStencilBuffer(const FrameBuffer::Settings& settings)
        {
            GLuint stencil = 0;
            
            if ((settings.stencil != FrameBuffer::eAttachement::None) && !settings.combinedDepthStencil)
            {
                Debug::LogError("Separate stencil buffer not implemented yet.");
            }

            GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
            if(status != GL_FRAMEBUFFER_COMPLETE)
            {
                Debug::LogError("Cannot create frame buffer.");
            }
            
            return stencil;
        }
    }
    
    FrameBuffersPool::~FrameBuffersPool()
    {
        for(const FrameBuffer& buffer : buffers)
        {
            DeleteBuffer(buffer);
        }
    }
    
    uint32_t FrameBuffersPool::GetBindTarget(FrameBuffer::eAttachement attachement)
    {
        if(attachement == FrameBuffer::eAttachement::Buffer)
        {
            return GL_RENDERBUFFER;
        }
        else if(attachement == FrameBuffer::eAttachement::Texture2d)
        {
            return GL_TEXTURE_2D;
        }
        else if(attachement == FrameBuffer::eAttachement::Texture2dArray)
        {
            return GL_TEXTURE_2D_ARRAY;
        }
        else if (attachement == FrameBuffer::eAttachement::Cubemap)
        {
            return GL_TEXTURE_CUBE_MAP;
        }
        
        return 0;
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
        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        GLuint colorBuffer = SFrameBuffersPool::BindColorBuffer(settings);
        Print_Errors_OpengGL();

        GLuint depth = SFrameBuffersPool::BindDepthBuffer(settings);
        Print_Errors_OpengGL();
        
        GLuint stencil = SFrameBuffersPool::BindStencilBuffer(settings);
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
