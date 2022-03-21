#pragma once

#include <Feng/Render/FrameBuffer.h>

namespace Feng
{
    class FrameBuffersPool final
    {
    public:
        FrameBuffersPool() = default;

        FrameBuffer CreateBuffer(uint32_t width, uint32_t height, bool depthStencil);
        void DeleteBuffer(const FrameBuffer& buffer);
    };
}
