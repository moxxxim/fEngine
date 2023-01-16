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

        FrameBuffer Pop(const FrameBuffer::Settings& settings);
        void Push(const FrameBuffer& buffer);
        void Clear();
        
    private:
        FrameBuffer CreateBuffer(const FrameBuffer::Settings& settings);
        void DeleteBuffer(const FrameBuffer& buffer);
        
        std::vector<FrameBuffer> buffers;
    };
}
