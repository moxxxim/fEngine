#pragma once

#include <Feng/ScenesManager/Fog.h>
#include <Feng/Math/Vector4.h>

namespace feng
{
    struct SceneSettings final
    {
        Fog fog;
        Vector4 aambientLightColor;
    };
}
