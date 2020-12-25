#pragma once

#include <cstdint>

namespace feng
{
    struct FrameBuffer final
    {
        uint32_t Frame = 0;
        uint32_t Color = 0; // Texture
        uint32_t DepthStencil = 0; // Renderbuffer.
    };
}
