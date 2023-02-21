#include <Feng/Render/PostEffects/Core/PostEffectPass.h>

#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Render/FrameBuffersPool.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace Feng
{
    PostEffectPass::PostEffectPass(
        Material *aMaterial,
        std::map<std::string, uint32_t> *aShaderInputTextures,
        int32_t aCustomBuffersCount,
        FrameBuffersPool *aBuffersPool)
        : material(aMaterial)
        , shaderInputTextures(aShaderInputTextures)
        , customBuffersCount(aCustomBuffersCount)
        , buffersPool(aBuffersPool)
    {
        if(customBuffersCount > 0)
        {
            buffers.reserve(customBuffersCount);
        }
    }

    void PostEffectPass::SetTextureFromOriginal(InputTextureType type, const char *textureName)
    {
        uint32_t textureBuffer = (type == PostEffectPass::InputTextureType::Color)
                ? context.original.color
                : context.original.depth;
        SetShaderInput(textureName, textureBuffer);
    }

    void PostEffectPass::SetTextureFromInput(InputTextureType type, const char *textureName)
    {
        uint32_t textureBuffer = (type == PostEffectPass::InputTextureType::Color)
                ? context.input.color
                : context.input.depth;
        SetShaderInput(textureName, textureBuffer);
    }

    void PostEffectPass::SetTextureFromCustom(InputTextureType type, const char *textureName, int textureBufferIndex)
    {
        if ((0 <= textureBufferIndex) && (textureBufferIndex < customBuffersCount))
        {
            const FrameBuffer& buffer = buffers[textureBufferIndex];
            uint32_t textureBuffer = (type == PostEffectPass::InputTextureType::Color)
                                    ? buffer.color
                                    : buffer.depth;
            SetShaderInput(textureName, textureBuffer);
        }
    }

    void PostEffectPass::SetRenderBufferCustom(int textureBufferIndex)
    {
        if ((0 <= textureBufferIndex) && (textureBufferIndex < customBuffersCount))
        {
            const FrameBuffer& buffer = buffers[textureBufferIndex];
            glBindFramebuffer(GL_FRAMEBUFFER, buffer.frame);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void PostEffectPass::SetRenderBufferOutput()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, context.output.frame);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PostEffectPass::Setup(const PostEffectContext& aContext)
    {
        context = aContext;
        for (int i = 0; i < customBuffersCount; ++i)
        {
            FrameBuffer::Settings bufferSettings = context.input.settings;
            bufferSettings.color = FrameBuffer::eAttachementState::Texture;
            bufferSettings.depth = FrameBuffer::eAttachementState::None;
            bufferSettings.stencil = FrameBuffer::eAttachementState::None;
            bufferSettings.multisample = false;
            bufferSettings.combinedDepthStencil = false;
            buffers[i] = buffersPool->Pop(bufferSettings);
        }
    }

    void PostEffectPass::Reset()
    {
        for (int i = 0; i < customBuffersCount; ++i)
        {
            buffersPool->Push(buffers[i]);
        }
    }

    void PostEffectPass::SetShaderInput(const char *textureName, unsigned int buffer)
    {
        (*shaderInputTextures)[textureName] = buffer;
    }
}
