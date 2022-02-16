#include <Classes/Scripts/GameObjectRotation.h>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MathUtils.h>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

void GameObjectRotation::Update(float deltaTime)
{
    if(!myTransform)
    {
        feng::Entity *myEntity = GetEntity();
        myTransform = myEntity->GetComponent<feng::Transform>();
    }

    myTransform->Rotate(0.f, 0.f, feng::RadToDeg(2.f * feng::Pi) * deltaTime / fullTurnPeriod);
}
