#pragma once

#include <Feng/Math/Vector3.h>
#include <Feng/ScenesManager/Component.h>
#include <Feng/ScenesManager/Space.h>

namespace Feng
{
    class Transform;
}

class GameObjectRotation final : public Feng::Component
{
public:
    static constexpr float ThresholdPeriod = 0.0001f;
    
	GameObjectRotation() = default;
    
    float GetPeriod() const;
    void SetPeriod(float aPeriod);
    
    const Feng::Vector3& GetAxis() const;
    void SetAxis(const Feng::Vector3& aAxis);
    
    Feng::eSpace GetSpace() const;
    void SetSpace(Feng::eSpace aSpace);

private:
    void Update(float deltaTime) override;
    
    Feng::Transform *myTransform = nullptr;
    Feng::Vector3 axis = Feng::Vector3{1.f, 1.f, 1.f};
	float period = GameObjectRotation::ThresholdPeriod;
    Feng::eSpace space = Feng::eSpace::Self;
};

inline float GameObjectRotation::GetPeriod() const
{
    return period;
}

inline void GameObjectRotation::SetPeriod(float aPeriod)
{
    period = aPeriod;
}

inline const Feng::Vector3& GameObjectRotation::GetAxis() const
{
    return axis;
}

inline void GameObjectRotation::SetAxis(const Feng::Vector3& aAxis)
{
    axis = aAxis.ToNormalized();
}

inline Feng::eSpace GameObjectRotation::GetSpace() const
{
    return space;
}

inline void GameObjectRotation::SetSpace(Feng::eSpace aSpace)
{
    space = aSpace;
}
