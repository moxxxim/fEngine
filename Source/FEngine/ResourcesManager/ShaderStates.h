#pragma once

namespace fengine
{
    enum class ShaderStates : int
    {
        None = 0,
        Culling = 1 << 0,
        AlphaTest = 1 << 1,
        DepthTest = 1 << 2
    };
}
