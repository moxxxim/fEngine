#pragma once

#include <Feng/Render/PostEffects/PostEffectContext.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace feng
{
    class Material;
    class Shader;
    class FrameBuffersPool;

    class PostEffectPass final
    {
    public:
        enum class InputTextureType : uint32_t
        {
            Color = 0,
            DepthStencil = 1
        };

        PostEffectPass(
                       Material *aMaterial,
                       std::map<std::string, uint32_t>& aShaderInputTextures,
                       int32_t aCustomBuffersCount,
                       FrameBuffersPool *aBuffersPool);

        void SetTextureFromOriginal(InputTextureType type, const char *textureName);
        void SetTextureFromInput(InputTextureType type, const char *textureName);
        void SetTextureFromCustom(InputTextureType type, const char *textureName, int index);
        void SetRenderBufferCustom(int index);
        void SetRenderBufferOutput();
        Material * GetMaterial() const;

    private:
        void Setup(const PostEffectContext& aContext);
        void Reset();
        void SetShaderInput(const char *textureName, unsigned int buffer);

        PostEffectContext context;
        Material *material = nullptr;
        FrameBuffersPool *buffersPool = nullptr;
        std::map<std::string, uint32_t> shaderInputTextures;
        int customBuffersCount = 0;
        std::vector<FrameBuffer> buffers;
    };
}

namespace feng
{
    inline Material* PostEffectPass::GetMaterial() const
    {
        return material;
    }
}
