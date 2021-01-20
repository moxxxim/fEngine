#pragma once

namespace feng
{
    enum class eShaderStates : int
    {
        None = 0,
        Culling = 1 << 0,
        AlphaTest = 1 << 1,
        DepthTest = 1 << 2
    };
}
