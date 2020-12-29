#pragma once

#include <Feng/Render/PostEffects/PostEffect.h>

#include <memory>
#include <vector>

namespace feng
{
    class FrameBuffersPool;

    class CompositePostEffect final : public PostEffect
    {
    public:
        CompositePostEffect(std::vector<std::unique_ptr<PostEffect>> &&aEffects, FrameBuffersPool *aBuffersPool);
        ~CompositePostEffect();
        void Apply(const PostEffectContext& context) override;

    private:
        std::vector<std::unique_ptr<PostEffect>> effects;
        FrameBuffersPool *buffersPool = nullptr;
    };
}
