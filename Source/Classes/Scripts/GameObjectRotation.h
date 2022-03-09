#pragma once

#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Transform;
}

class GameObjectRotation final : public Feng::Component
{
public:
	GameObjectRotation() = default;

private:
    void Update(float deltaTime) override;
    
    Feng::Transform *myTransform = nullptr;
	float fullTurnPeriod = 4.f;
};
