#pragma once

#include <Feng/Render/FrameBuffer.h>

namespace Feng
{
    struct PostEffectContext final
    {
        PostEffectContext(FrameBuffer aOriginal, FrameBuffer aInput, FrameBuffer aOutput)
        : original(aOriginal)
        , input(aInput)
        , output(aOutput)
        { }

        PostEffectContext() = default;

        FrameBuffer original;
        FrameBuffer input;
        FrameBuffer output;
    };
}
