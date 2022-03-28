#include <Feng/Render/FrameBuffer.h>

namespace Feng
{
    bool FrameBuffer::Suits(uint32_t width, uint32_t height, bool depthStencil, bool multisample) const
    {
        return (Width == width)
            && (Height == height)
            && (depthStencil == (DepthStencil != FrameBuffer::Default))
            && (IsMultisample == multisample);
    }
}
