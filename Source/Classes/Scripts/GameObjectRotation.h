#pragma once

#include <Feng/ScenesManager/Component.h>

namespace feng
{
    class Transform;
}

class GameObjectRotation final : public feng::Component
{
public:
	GameObjectRotation() = default;

private:
    void Update(float deltaTime) override;
    
    feng::Transform *myTransform = nullptr;
	float fullTurnPeriod = 4.f;
};
