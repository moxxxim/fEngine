#include <Classes/Scripts/DirectionalLightMovement.hpp>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MathUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

void DirectionalLightMovement::Update(float deltaTime)
{
    if(!myTransform)
    {
        myTransform = GetEntity()->GetComponent<Feng::Transform>();
        start = myTransform->GetPosition();

        Feng::Vector3 startXz = start - Feng::Vector3{0.f, start.y, 0.f};
        radius = startXz.Length();
        height = start.y;
        yStartAngleRad = acos(Feng::Vector3::Dot(Feng::Vector3::OneZ, startXz.ToNormalized()));
        float acosX = acos(Feng::Vector3::Dot(Feng::Vector3::OneX, startXz.ToNormalized()));
        if(acosX > Feng::Math::Pi / 2)
        {
            yStartAngleRad *= -1;
        }
    }
    
    if(timeSegment > period)
    {
        timeSegment = 0.f;
    }

    timeSegment += deltaTime;
    float angleOffsetY = 2 * Feng::Math::Pi * (timeSegment / period);
    float yAngleRad = yStartAngleRad + angleOffsetY;
    
    Feng::Vector3 position = radius
                                * (Feng::Vector3::OneX * std::sin(yAngleRad) + Feng::Vector3::OneZ * std::cos(yAngleRad))
                                + height * Feng::Vector3::OneY;
    myTransform->SetPosition(position);

    Feng::Quaternion xRotation { Feng::Vector3::OneX, xAngleDeg };
    Feng::Quaternion yRotation { Feng::Vector3::OneY, Feng::Math::RadToDeg(yAngleRad) + 180};
    myTransform->SetRotation(xRotation * yRotation);
}
