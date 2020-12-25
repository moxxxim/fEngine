#pragma once

#include "PostEffectContext.h"

namespace feng
{
    class PostEffect
    {
    public:
        virtual ~PostEffect() {};
        virtual void Apply(PostEffectContext context) = 0;
    };
}
