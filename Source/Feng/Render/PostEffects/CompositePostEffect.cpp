#include <Feng/Render/PostEffects/CompositePostEffect.h>

#include <Feng/Render/FrameBuffersPool.h>
#include <cstdint>

namespace Feng
{
    CompositePostEffect::CompositePostEffect(std::vector<std::unique_ptr<PostEffect>> &&aEffects, FrameBuffersPool *aBuffersPool)
        : effects {std::move(aEffects)}
        , buffersPool {buffersPool}
    { }

    CompositePostEffect::~CompositePostEffect() = default;

    void CompositePostEffect::Apply(const PostEffectContext& context)
    {
        if (effects.size() > 1)
        {
            FrameBuffer intermediateBuffer = buffersPool->CreateBuffer(context.Input.Width, context.Input.Height, false);

            for (uint32_t i = 0; i < effects.size(); ++i)
            {
                FrameBuffer input = (i == 0) ? context.Input : intermediateBuffer;
                FrameBuffer output = (effects.size() - 1) ? context.Output : intermediateBuffer;
                PostEffectContext innerContext(context.Original, input, output);
                std::unique_ptr<PostEffect>& effect = effects[i];
                effect->Apply(innerContext);
            }
        }
        else if (!effects.empty())
        {
            effects[0]->Apply(context);
        }
    }
}
