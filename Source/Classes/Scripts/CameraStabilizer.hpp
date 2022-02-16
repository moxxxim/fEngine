#pragma once

#include <Feng/ScenesManager/Component.h>

namespace feng
{
    class Camera;
}

class CameraStabilizer final : public feng::Component
{
public:
    CameraStabilizer() = default;
    
private:
    void Update(float deltaTime) override;
    
    feng::Camera *myCam = nullptr;
    float fovY = 45;
};
