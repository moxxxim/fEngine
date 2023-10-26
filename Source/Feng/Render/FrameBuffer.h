#pragma once

#include <Feng/Math/Size.h>
#include <cstdint>
#include <vector>

namespace Feng
{
    class FrameBuffer final
    {
    public:
        static constexpr uint32_t Default = 0;

        enum class eAttachement
        {
            None = 0,
            Buffer = 1,
            Texture2d = 2,
            Cubemap = 3,
            Texture2dArray = 4,
        };
        
        struct Settings final
        {
            Size2ui size {};
            eAttachement color = eAttachement::None;
            eAttachement depth = eAttachement::None;
            eAttachement stencil = eAttachement::None;
            uint32_t colorBuffersCount = 1;
            uint32_t depth2dArraySize = 0;
            bool multisample = false;
            bool combinedDepthStencil = false;
            bool hdr = false;
        };

        uint32_t frame = Default;
        std::vector<uint32_t> color;
        uint32_t depth = Default;
        uint32_t stencil = Default;

        FrameBuffer::Settings settings;
    };
    
    inline bool operator==(const FrameBuffer::Settings& lhs, const FrameBuffer::Settings& rhs)
    {
        return (lhs.size == rhs.size)
            && (lhs.color == rhs.color)
            && (lhs.depth == rhs.depth)
            && (lhs.stencil == rhs.stencil)
            && (lhs.colorBuffersCount == rhs.colorBuffersCount)
            && (lhs.depth2dArraySize == rhs.depth2dArraySize)
            && (lhs.multisample == rhs.multisample)
            && (lhs.combinedDepthStencil == rhs.combinedDepthStencil)
            && (lhs.hdr == rhs.hdr);
    }
    
    inline bool operator!=(const FrameBuffer::Settings& lhs, const FrameBuffer::Settings& rhs)
    {
        return !(lhs == rhs);
    }
}
