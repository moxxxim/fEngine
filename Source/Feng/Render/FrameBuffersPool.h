#pragma once

#include <Feng/Render/FrameBuffer.h>

#include <vector>

namespace feng
{
    class FrameBuffersPool final
    {
    public:
        FrameBuffersPool() = default;

        FrameBuffer CreateBuffer(bool depthStencil);
        void DeleteBuffer(const FrameBuffer& buffer);

    private:
        std::vector<FrameBuffer> buffers;
    };
}
