#pragma once

#include <Feng/Render/FrameBuffer.h>
#include <vector>

namespace Feng
{
    class FrameBuffersPool final
    {
    public:
        FrameBuffersPool() = default;
        ~FrameBuffersPool();

        FrameBuffer Pop(uint32_t width, uint32_t height, bool depthStencil, bool multisample);
        void Push(const FrameBuffer& buffer);
        void Clear();
        
    private:
        FrameBuffer CreateBuffer(uint32_t width, uint32_t height, bool depthStencil, bool multisample);
        void DeleteBuffer(const FrameBuffer& buffer);
        
        std::vector<FrameBuffer> buffers;
    };
}
