#pragma once

#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Camera;
}

class CameraStabilizer final : public Feng::Component
{
public:
    CameraStabilizer() = default;
    void SetMinMaxFov(float min, float max);
    
private:
    void Update(float deltaTime) override;
    
    Feng::Camera *myCam = nullptr;
    float minFovY = 45;
    float maxFovY = 45;
    float fovY = 45;
};
