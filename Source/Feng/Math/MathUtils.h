#pragma once

#include <math.h>

namespace feng
{
    constexpr float DegToRad(float degrees)
    {
        return degrees * M_PI / 180.f;
    }

    constexpr float RadToDeg(float rad)
    {
        return rad * 180.f / M_PI;
    }
}
