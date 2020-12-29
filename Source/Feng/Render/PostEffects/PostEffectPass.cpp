#include <Feng/Render/PostEffects/PostEffectPass.h>

#include <Feng/ResourcesManager/Material.h>
#include <Feng/ResourcesManager/Shader.h>
#include <Feng/Render/FrameBuffersPool.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace feng
{
    PostEffectPass::PostEffectPass(
        Material *aMaterial,
        std::map<std::string, uint32_t>& aShaderInputTextures,
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
        uint32_t textureBuffer = (type == feng::PostEffectPass::InputTextureType::Color)
                ? context.Original.Color
                : context.Original.DepthStencil;
        SetShaderInput(textureName, textureBuffer);
    }

    void PostEffectPass::SetTextureFromInput(InputTextureType type, const char *textureName)
    {
        uint32_t textureBuffer = (type == feng::PostEffectPass::InputTextureType::Color)
                ? context.Input.Color
                : context.Input.DepthStencil;
        SetShaderInput(textureName, textureBuffer);
    }

    void PostEffectPass::SetTextureFromCustom(InputTextureType type, const char *textureName, int textureBufferIndex)
    {
        if ((0 <= textureBufferIndex) && (textureBufferIndex < customBuffersCount))
        {
            const FrameBuffer& buffer = buffers[textureBufferIndex];
            uint32_t textureBuffer = (type == feng::PostEffectPass::InputTextureType::Color) ? buffer.Color : buffer.DepthStencil;
            SetShaderInput(textureName, textureBuffer);
        }
    }

    void PostEffectPass::SetRenderBufferCustom(int textureBufferIndex)
    {
        if ((0 <= textureBufferIndex) && (textureBufferIndex < customBuffersCount))
        {
            const FrameBuffer& buffer = buffers[textureBufferIndex];
            glBindFramebuffer(GL_FRAMEBUFFER, buffer.Frame);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }
    }

    void PostEffectPass::SetRenderBufferOutput()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, context.Output.Frame);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void PostEffectPass::Setup(const PostEffectContext& aContext)
    {
        context = aContext;
        for (int i = 0; i < customBuffersCount; ++i)
        {
            buffers[i] = buffersPool->CreateBuffer(context.Input.Width, context.Input.Height, false);
        }
    }

    void PostEffectPass::Reset()
    {
        for (int i = 0; i < customBuffersCount; ++i)
        {
            buffersPool->DeleteBuffer(buffers[i]);
        }
    }

    void PostEffectPass::SetShaderInput(const char *textureName, unsigned int buffer)
    {
        shaderInputTextures[textureName] = buffer;
    }
}
