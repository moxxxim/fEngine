#pragma once

#include <Feng/Render/FrameBuffer.h>

namespace feng
{
    struct PostEffectContext final
    {
        PostEffectContext(FrameBuffer original, FrameBuffer input, FrameBuffer output)
        : Original(original)
		, Input(input)
		, Output(output)
        { }

        PostEffectContext() = default;

        FrameBuffer Original;
        FrameBuffer Input;
        FrameBuffer Output;
    };
}
