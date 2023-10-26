#pragma once

#include <math.h>
#include <numbers>

namespace Feng::Math
{
    constexpr const float Pi = std::numbers::pi_v<float>;
    
    constexpr float DegToRad(float degrees)
    {
        return degrees * Pi / 180.f;
    }

    constexpr float RadToDeg(float rad)
    {
        return rad * 180.f / Pi;
    }
}
