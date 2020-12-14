#pragma once

#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>

namespace feng
{
    class Light;
    class Camera;

    struct RenderProperties final
    {
        const Camera *cam = nullptr;
        const Light *pointLight = nullptr;
        Vector4 ambientColor = Vector4{1.f, 1.f, 1.f, 1.f};
    };
}
