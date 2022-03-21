#include <Classes/Scripts/GameObjectRotation.h>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

void GameObjectRotation::Update(float deltaTime)
{
    if(std::abs(period) > GameObjectRotation::ThresholdPeriod)
    {
        if(!myTransform)
        {
            Feng::Entity *myEntity = GetEntity();
            myTransform = myEntity->GetComponent<Feng::Transform>();
        }

        float dAngle = Feng::Math::RadToDeg(2.f * Feng::Math::Pi) * deltaTime / period;
        myTransform->Rotate(axis, dAngle, space);
    }
}
