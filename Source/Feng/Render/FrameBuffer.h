#pragma once

#include <cstdint>

namespace Feng
{
    struct FrameBuffer final
    {
        static constexpr uint32_t Default = 0;
        
        uint32_t Frame = Default; // Zero - is default buffer (render to screen).
        uint32_t Color = Default; // Texture
        uint32_t DepthStencil = Default; // Renderbuffer.

        uint32_t Width = 0;
        uint32_t Height = 0;
    };
}
