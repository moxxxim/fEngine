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
        const Light *directLight = nullptr;
        const Light *pointLight = nullptr;
        const Light *spotLight = nullptr;
        Vector4 ambientColorAndIntencity = Vector4{1.f, 1.f, 1.f, 1.f};
    };
}
