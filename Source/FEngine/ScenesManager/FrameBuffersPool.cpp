//#include <FEngine/ScenesManager/FrameBuffersPool.h>
//
//#include <Classes/Globals.h>
//
//namespace feng
//{
//    FrameBuffersPool::FrameBuffersPool() :
//        m_buffers(4)
//    { }
//
//    FrameBuffersPool::~FrameBuffersPool()
//    {
//        unsigned int buffersCount = m_buffers.GetSize();
//        for (unsigned i = 0; i < buffersCount; ++i)
//        {
//            FrameBuffer buffer = m_buffers[i];
//            glDeleteFramebuffers(1, &buffer.Frame);
//            glDeleteTextures(1, &buffer.Color);
//            glDeleteTextures(1, &buffer.Depth);
//        }
//    }
//
//    void FrameBuffersPool::Prewarm(int count, bool requireDepth)
//    {
//        unsigned buffersCount = m_buffers.GetSize();
//        if (requireDepth > buffersCount)
//        {
//            for (int i = 0; i < requireDepth - buffersCount; ++i)
//            {
//                FrameBuffer buffer = CreateBuffer(requireDepth);
//                m_buffers.Add(buffer);
//            }
//        }
//    }
//
//    FrameBuffer FrameBuffersPool::GetBuffer(bool requireDepth)
//    {
//        FrameBuffer buffer;
//        if (TryGetRequiredBuffer(requireDepth, buffer))
//        {
//            return buffer;
//        }
//
//        return CreateBuffer(requireDepth);
//    }
//
//    void FrameBuffersPool::PutBuffer(FrameBuffer buffer)
//    {
//        m_buffers.Add(buffer);
//    }
//
//    bool FrameBuffersPool::TryGetRequiredBuffer(bool requireDepth, FrameBuffer& buffer)
//    {
//        if (!m_buffers.IsEmpty())
//        {
//            if (requireDepth)
//            {
//                unsigned buffersCount = m_buffers.GetSize();
//                for (unsigned i = 0; i < buffersCount; ++i)
//                {
//                    buffer = m_buffers[i];
//                    if (buffer.Depth)
//                    {
//                        m_buffers.RemoveAt(i);
//                        return true;
//                    }
//                }
//            }
//            else
//            {
//                buffer = m_buffers[0];
//                m_buffers.RemoveAt(0);
//                return true;
//            }
//        }
//
//        return false;
//    }
//
//    FrameBuffer FrameBuffersPool::CreateBuffer(bool requireDepth)
//    {
//        GLuint colorBuffer;
//
//        glGenTextures(1, &colorBuffer);
//        glBindTexture(GL_TEXTURE_2D, colorBuffer);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Globals::screenWidth, Globals::screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
//        glBindTexture(GL_TEXTURE_2D, 0);
//
//        GLuint depthBuffer = 0;
//        if (requireDepth)
//        {
//            glGenTextures(1, &depthBuffer);
//            glBindTexture(GL_TEXTURE_2D, depthBuffer);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, Globals::screenWidth, Globals::screenHeight, 0, GL_DEPTH_COMPONENT, GL_UNSIGNED_SHORT, NULL);
//            glBindTexture(GL_TEXTURE_2D, 0);
//        }
//
//        GLuint frameBuffer;
//        glGenFramebuffers(1, &frameBuffer);
//        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer);
//        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBuffer, 0);
//
//        if (requireDepth)
//        {
//            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBuffer, 0);
//        }
//
//        glBindFramebuffer(GL_FRAMEBUFFER, 0);
//
//        auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
//        switch (status)
//        {
//            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
//                esLogMessage("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT");
//                break;
//
//            case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS:
//                esLogMessage("GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS");
//                break;
//
//            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
//                esLogMessage("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT");
//                break;
//
//            case GL_FRAMEBUFFER_UNSUPPORTED:
//                esLogMessage("GL_FRAMEBUFFER_UNSUPPORTED");
//                break;
//        }
//
//        return FrameBuffer(frameBuffer, colorBuffer, depthBuffer);
//    }
//}
