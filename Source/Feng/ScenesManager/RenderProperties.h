#pragma once

namespace feng
{
    class Light;
    class Camera;

    struct RenderProperties final
    {
        const Camera *cam = nullptr;
        const Light *light = nullptr;
    };
}
