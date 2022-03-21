#pragma once

#include <cstdint>

namespace Feng
{
    enum class eDrawFace : uint8_t
    {
        Cw = 0,
        Ccw = 1,
        Both = 2,
    };
    
    namespace Render
    {
        static constexpr uint32_t UndefinedBuffer = 0;
        
        void SetDrawFace(eDrawFace face);
        void SetBlending(bool transparent);
    }
}
