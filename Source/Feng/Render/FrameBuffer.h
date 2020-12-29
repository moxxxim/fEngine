#pragma once

#include <cstdint>

namespace feng
{
    struct FrameBuffer final
    {
        uint32_t Frame = 0; // Zero - is default buffer (render to screen).
        uint32_t Color = 0; // Texture
        uint32_t DepthStencil = 0; // Renderbuffer.
    };
}
