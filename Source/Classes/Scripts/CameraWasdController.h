#pragma once

#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Transform;
}

class CameraWasdController final : public Feng::Component
{
public:
    CameraWasdController() = default;
    
private:
    void Update(float deltaTime) override;
    
    void UpdatePosition(float deltaTime);
    void UpdateRotationArrows(float deltaTime);
    void UpdateRotationMouse();

	Feng::Transform *myTransform = nullptr;
    float speed = 3.f;
    float speedMultiplier = 1;
    float camSensitivity = 0.1f;
    float pitch = 0.f;
    float yaw = 0.f;
    float lastMouseX = 0;
    float lastMouseY = 0;
};
