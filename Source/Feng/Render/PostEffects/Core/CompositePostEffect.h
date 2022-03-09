#pragma once

#include <Feng/Render/PostEffects/Core/PostEffect.h>

#include <memory>
#include <vector>

namespace Feng
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
