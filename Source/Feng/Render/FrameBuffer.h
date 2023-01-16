#pragma once

#include <cstdint>

namespace Feng
{
    class FrameBuffer final
    {
    public:
        static constexpr uint32_t Default = 0;

        enum class eAttachementState
        {
            None = 0,
            Texture = 1,
            Buffer = 2,
        };
        
        struct Settings final
        {
            uint32_t width = 0;
            uint32_t height = 0;
            eAttachementState color = eAttachementState::None;
            eAttachementState depth = eAttachementState::None;
            eAttachementState stencil = eAttachementState::None;
            bool multisample = false;
            bool combinedDepthStencil = false;
        };
        
        uint32_t frame = Default;
        uint32_t color = Default;
        uint32_t depth = Default;
        uint32_t stencil = Default;
        
        FrameBuffer::Settings settings;
    };
    
    inline bool operator==(const FrameBuffer::Settings& lhs, const FrameBuffer::Settings& rhs)
    {
        return (lhs.width == rhs.width)
            && (lhs.height == rhs.height)
            && (lhs.color == rhs.color)
            && (lhs.depth == rhs.depth)
            && (lhs.stencil == rhs.stencil)
            && (lhs.multisample == rhs.multisample)
            && (lhs.combinedDepthStencil == rhs.combinedDepthStencil);
    }
    
    inline bool operator!=(const FrameBuffer::Settings& lhs, const FrameBuffer::Settings& rhs)
    {
        return !(lhs == rhs);
    }
}
