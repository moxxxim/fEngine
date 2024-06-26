#include <Feng/Render/PostEffects/Core/CompositePostEffect.h>

#include <Feng/Render/FrameBuffersPool.h>
#include <cstdint>

namespace Feng
{
    CompositePostEffect::CompositePostEffect(std::vector<std::unique_ptr<PostEffect>> &&aEffects, FrameBuffersPool *aBuffersPool)
        : effects { std::move(aEffects) }
        , buffersPool { aBuffersPool }
    { }

    CompositePostEffect::~CompositePostEffect() = default;

    void CompositePostEffect::Apply(const PostEffectContext& context)
    {
        if (effects.size() > 1)
        {
            FrameBuffer::Settings bufferSettings;
            bufferSettings.size = context.input.settings.size;
            FrameBuffer intermediateBuffer = buffersPool->Pop(bufferSettings);

            for (uint32_t i = 0; i < effects.size(); ++i)
            {
                FrameBuffer input = (i == 0) ? context.input : intermediateBuffer;
                FrameBuffer output = (effects.size() - 1) ? context.output : intermediateBuffer;
                PostEffectContext innerContext(context.original, input, output);
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
