#pragma once

#include <Feng/Math/Vector3.h>
#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Transform;
}

class GameObjectSliderX final : public Feng::Component
{
public:
    GameObjectSliderX() = default;
    
private:
    void Update(float deltaTime) override;

    Feng::Transform *myTransform = nullptr;
    Feng::Vector3 start;
    float amplitude = 5;
};
