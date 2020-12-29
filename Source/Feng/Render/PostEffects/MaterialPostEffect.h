#pragma once

#include <Feng/Render/PostEffects/PostEffect.h>

#include <cstdint>
#include <map>
#include <memory>
#include <string>

namespace feng
{
    class FrameBuffersPool;
    class PostEffectRenderer;
    class PostEffectPassBehaviour;
    class PostEffectPass;
    class Material;

    class MaterialPostEffect final : public PostEffect
    {
    public:
        MaterialPostEffect(Material *aMaterial, FrameBuffersPool *aBuffersPool);
        MaterialPostEffect(
                    Material *aMaterial,
                    FrameBuffersPool *aBuffersPool,
                    PostEffectPassBehaviour *aPassBehaviour);

        ~MaterialPostEffect() override;

        void Apply(const PostEffectContext& context) override;

    private:
        void Draw();
        void ApplyCustomBehaviour();
        void ApplyGenericBehaviour();

        PostEffectPassBehaviour *passBehaviour = nullptr;
        std::unique_ptr<PostEffectRenderer> renderer;
        std::unique_ptr<PostEffectPass> pass;
        std::map<std::string, uint32_t> shaderInputTextures;
    };
}
