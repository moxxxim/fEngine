#include <Feng/Render/FrameBuffersPool.h>

#include <Feng/App/Globals.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    FrameBuffer FrameBuffersPool::CreateBuffer(uint32_t width, uint32_t height, bool depthStencil)
    {
        GLenum target = GL_TEXTURE_2D;

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

        return frameBuffer;
    }

    void FrameBuffersPool::DeleteBuffer(const FrameBuffer& buffer)
    {
        glDeleteFramebuffers(1, &buffer.Frame);
        glDeleteTextures(1, &buffer.Color);
        glDeleteRenderbuffers(1, &buffer.DepthStencil);
    }
}
