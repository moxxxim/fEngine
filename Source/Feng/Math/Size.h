#pragma once

#include <cstdint>

namespace Feng
{
    struct Size2ui final
    {
        uint32_t width = 0;
        uint32_t height = 0;
        
        friend bool operator == (const Size2ui& lhs, const Size2ui& rhs);
        friend bool operator != (const Size2ui& lhs, const Size2ui& rhs);
    };
    
    inline bool operator == (const Size2ui& lhs, const Size2ui& rhs)
    {
        return (lhs.width == rhs.width) && (lhs.height == rhs.height);
    }
    
    inline bool operator != (const Size2ui& lhs, const Size2ui& rhs)
    {
        return !(lhs == rhs);
    }
}
