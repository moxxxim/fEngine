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
        Feng::Entity *myEntity = GetEntity();
        myTransform = myEntity->GetComponent<Feng::Transform>();
    }

    myTransform->Rotate(0.f, 0.f, Feng::RadToDeg(2.f * Feng::Pi) * deltaTime / fullTurnPeriod);
}
