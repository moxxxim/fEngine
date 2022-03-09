#pragma once

#include <Feng/Render/PostEffects/PostEffectContext.h>

namespace Feng
{
    class PostEffect
    {
    public:
        virtual ~PostEffect() = default;
        virtual void Apply(const PostEffectContext &context) = 0;
    };
}
