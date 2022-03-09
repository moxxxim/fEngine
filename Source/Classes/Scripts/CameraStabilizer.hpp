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
    
private:
    void Update(float deltaTime) override;
    
    Feng::Camera *myCam = nullptr;
    float fovY = 45;
};
