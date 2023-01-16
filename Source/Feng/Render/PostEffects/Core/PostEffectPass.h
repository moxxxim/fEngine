#pragma once

#include <Feng/Render/PostEffects/Core/PostEffectContext.h>

#include <cstdint>
#include <map>
#include <string>
#include <vector>

namespace Feng
{
    class Material;
    class FrameBuffersPool;

    class PostEffectPass final
    {
    public:
        enum class InputTextureType : uint32_t
        {
            Color = 0,
            Depth = 1
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
        Material* GetMaterial() const;

        void Setup(const PostEffectContext& aContext);
        void Reset();
        void SetShaderInput(const char *textureName, unsigned int buffer);

    private:
        PostEffectContext context;
        Material *material = nullptr;
        FrameBuffersPool *buffersPool = nullptr;
        std::map<std::string, uint32_t>& shaderInputTextures;
        int customBuffersCount = 0;
        std::vector<FrameBuffer> buffers;
    };
}

namespace Feng
{
    inline Material* PostEffectPass::GetMaterial() const
    {
        return material;
    }
}
