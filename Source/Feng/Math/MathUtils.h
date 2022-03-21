#pragma once

#include <math.h>

namespace Feng::Math
{
    constexpr const float Pi = M_PI;
    
    constexpr float DegToRad(float degrees)
    {
        return degrees * Pi / 180.f;
    }

    constexpr float RadToDeg(float rad)
    {
        return rad * 180.f / Pi;
    }
}
