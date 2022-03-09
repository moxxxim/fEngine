#pragma once

#include <Feng/Math/Vector4.h>

namespace Feng
{
    struct Fog final
    {
    public:
        float start = 0.f;
        float range = 0.f;
        float density = 0.f;
        Vector4 color = Vector4(1.f, 1.f, 1.f, 1.f);
    };
}
