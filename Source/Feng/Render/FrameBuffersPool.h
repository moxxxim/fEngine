#pragma once

#include <Feng/Math/Size.h>
#include <Feng/Render/FrameBuffer.h>
#include <vector>

namespace Feng
{
    class FrameBuffersPool final
    {
    public:
        FrameBuffersPool() = default;
        ~FrameBuffersPool();

        static uint32_t GetBindTarget(FrameBuffer::eAttachement attachement);
        FrameBuffer Pop(const FrameBuffer::Settings& settings);
        void Push(const FrameBuffer& buffer);
        
    private:
        FrameBuffer CreateBuffer(const FrameBuffer::Settings& settings);
        void DeleteBuffer(const FrameBuffer& buffer);
        
        std::vector<FrameBuffer> buffers;
    };
}
