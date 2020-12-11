//#include <Feng/ScenesManager/PostEffectPass.h>
//
//#include <Feng/ResourcesManager/Material.h>
//#include <Feng/ScenesManager/FrameBuffersPool.h>
//
//namespace feng
//{
//    PostEffectPass::PostEffectPass(
//        Material *material,
//        std::map<const char *, unsigned>& shaderInputTextures,
//        int customBuffersCount,
//        FrameBuffersPool *buffersPool) :
//        m_material(material),
//        m_shaderInputTextures(shaderInputTextures),
//        m_customBuffersCount(customBuffersCount),
//        m_buffersPool(buffersPool)
//    {
//        m_buffers = customBuffersCount > 0 ? new FrameBuffer[m_customBuffersCount] : nullptr;
//    }
//
//    PostEffectPass::~PostEffectPass()
//    {
//        delete[] m_buffers;
//    }
//
//    void PostEffectPass::Setup(const PostEffectContext& context)
//    {
//        m_context = context;
//        for (int i = 0; i < m_customBuffersCount; ++i)
//        {
//            m_buffers[i] = m_buffersPool->GetBuffer(false);
//        }
//    }
//
//    void PostEffectPass::Reset()
//    {
//        for (int i = 0; i < m_customBuffersCount; ++i)
//        {
//            m_buffersPool->PutBuffer(m_buffers[i]);
//        }
//    }
//
//    void PostEffectPass::SetTextureFromOriginal(InputTextureType type, const char *textureName)
//    {
//        GLuint textureBuffer = type == feng::PostEffectPass::InputTextureType::Color ? m_context.Original.Color : m_context.Original.Depth;
//        SetShaderInput(textureName, textureBuffer);
//    }
//
//    void PostEffectPass::SetTextureFromInput(InputTextureType type, const char *textureName)
//    {
//        GLuint textureBuffer = type == feng::PostEffectPass::InputTextureType::Color ? m_context.Input.Color : m_context.Input.Depth;
//        SetShaderInput(textureName, textureBuffer);
//    }
//
//    void PostEffectPass::SetTextureFromCustom(InputTextureType type, const char *textureName, int textureBufferIndex)
//    {
//        if (textureBufferIndex < m_customBuffersCount && textureBufferIndex >= 0)
//        {
//            FrameBuffer buffer = m_buffers[textureBufferIndex];
//            GLuint textureBuffer = type == feng::PostEffectPass::InputTextureType::Color ? buffer.Color : buffer.Depth;
//            SetShaderInput(textureName, textureBuffer);
//        }
//    }
//
//    void PostEffectPass::SetRenderBufferCustom(int textureBufferIndex)
//    {
//        if (textureBufferIndex < m_customBuffersCount && textureBufferIndex >= 0)
//        {
//            FrameBuffer buffer = m_buffers[textureBufferIndex];
//            glBindFramebuffer(GL_FRAMEBUFFER, buffer.Frame);
//            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        }
//    }
//
//    void PostEffectPass::SetRenderBufferOutput()
//    {
//        glBindFramebuffer(GL_FRAMEBUFFER, m_context.Output.Frame);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    }
//
//    void PostEffectPass::SetShaderInput(const char *textureName, unsigned int buffer)
//    {
//        if (m_shaderInputTextures.HasKey(textureName))
//        {
//            m_shaderInputTextures[textureName] = buffer;
//        }
//        else
//        {
//            m_shaderInputTextures.Add(textureName, buffer);
//        }
//    }
//}
