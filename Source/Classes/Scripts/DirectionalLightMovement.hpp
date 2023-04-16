#pragma once

#include <Feng/Math/Vector3.h>
#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Transform;
}

class DirectionalLightMovement final : public Feng::Component
{
public:
    DirectionalLightMovement() = default;
    
private:
    void Update(float deltaTime) override;

    Feng::Transform *myTransform = nullptr;
    Feng::Vector3 start;
    float xAngleDeg = -40.f;
    float radius = 0.f;
    float height = 0.f;
    float yStartAngleRad = 0.f;
    float period = 30.f;
    float timeSegment = 0.f;
    float timeScale = 1.f;
};
