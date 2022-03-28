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
    
    FrameBuffer FrameBuffersPool::Pop(uint32_t width, uint32_t height, bool depthStencil, bool multisample)
    {
        auto isSuitable = [width,height, depthStencil, multisample](const FrameBuffer& buffer)
        {
            return buffer.Suits(width, height, depthStencil, multisample);
        };
        
        auto it = std::find_if(buffers.begin(), buffers.end(), isSuitable);
        if(it != buffers.end())
        {
            FrameBuffer outBuffer = *it;
            std::swap(*it, buffers.back());
            buffers.pop_back();
            
            return outBuffer;
        }
        
        return CreateBuffer(width, height, depthStencil, multisample);
    }

    FrameBuffer FrameBuffersPool::CreateBuffer(uint32_t width, uint32_t height, bool depthStencil, bool multisample)
    {
        GLenum target = multisample ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;

        GLuint fbo;
        glGenFramebuffers(1, &fbo);
        glBindFramebuffer(GL_FRAMEBUFFER, fbo);

        GLuint colorBuffer;
        glGenTextures(1, &colorBuffer);
        glBindTexture(target, colorBuffer);

        if(target == GL_TEXTURE_2D)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        }
        else if (target == GL_TEXTURE_2D_MULTISAMPLE)
        {
            GLsizei samplesCount = 4;
            glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samplesCount, GL_RGB, width, height, GL_TRUE);
        }

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, target, colorBuffer, 0);

        GLuint depthStencilBuffer = 0;
        if (depthStencil)
        {
            glGenRenderbuffers(1, &depthStencilBuffer);
            glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
            if(target == GL_TEXTURE_2D)
            {
                glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
            }
            else if (target == GL_TEXTURE_2D_MULTISAMPLE)
            {
                GLsizei samplesCount = 4;
                glRenderbufferStorageMultisample(GL_RENDERBUFFER, samplesCount, GL_DEPTH24_STENCIL8, width, height);
            }

            glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBuffer);
        }

        GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if(status != GL_FRAMEBUFFER_COMPLETE)
        {
            Debug::LogError("Cannot create frame buffer.");
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        FrameBuffer frameBuffer;
        frameBuffer.Frame = fbo;
        frameBuffer.Color = colorBuffer;
        frameBuffer.DepthStencil = depthStencilBuffer;
        frameBuffer.Width = width;
        frameBuffer.Height = height;
        frameBuffer.IsMultisample = multisample;

        Print_Errors_OpengGL();
        return frameBuffer;
    }
    
    void FrameBuffersPool::Push(const FrameBuffer& buffer)
    {
        buffers.push_back(buffer);
    }
    
    void FrameBuffersPool::DeleteBuffer(const FrameBuffer& buffer)
    {
        glDeleteFramebuffers(1, &buffer.Frame);
        glDeleteTextures(1, &buffer.Color);
        glDeleteRenderbuffers(1, &buffer.DepthStencil);
    }
}
