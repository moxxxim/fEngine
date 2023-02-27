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
        
        Camera *cam = nullptr;
        Light *directLight = nullptr;
        Light *pointLight = nullptr;
        Light *spotLight = nullptr;
        Entity *directShadowLight = nullptr;
        Entity *pointShadowLight = nullptr;
        Vector4 ambientColorAndIntencity = Vector4 {1.f, 1.f, 1.f, 1.f};
        bool useShadows = false;
    };
}
