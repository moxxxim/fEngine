#pragma once

#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>
#include <Feng/ScenesManager/Entity.h>

namespace Feng
{
    class Light;
    class Camera;

    struct RenderProperties final
    {
        static constexpr uint32_t CamBufferBinding = 0;
        
        const Camera *cam = nullptr;
        const Light *directLight = nullptr;
        const Light *pointLight = nullptr;
        const Light *spotLight = nullptr;
        const Entity *shadowLight = nullptr;
        Vector4 ambientColorAndIntencity = Vector4 {1.f, 1.f, 1.f, 1.f};
        bool useShadows = false;
    };
}
