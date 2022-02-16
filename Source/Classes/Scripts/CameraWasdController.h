#pragma once

#include <Feng/ScenesManager/Component.h>

namespace feng
{
    class Transform;
}

class CameraWasdController final : public feng::Component
{
public:
    CameraWasdController() = default;
    
private:
    void Update(float deltaTime) override;
    
    void UpdatePosition(float deltaTime);
    void UpdateRotation();

	feng::Transform *myTransform = nullptr;
    float speed = 3.f;
    float speedMultiplier = 1;
    float camSensitivity = 0.1f;
    float pitch = 0.f;
    float yaw = 0.f;
    float lastMouseX = 0;
    float lastMouseY = 0;
};
