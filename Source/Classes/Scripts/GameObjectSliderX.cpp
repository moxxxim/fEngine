#include <Classes/Scripts/GameObjectSliderX.hpp>

#include <Feng/Core/Engine.hpp>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>
#include <Feng/Utils/Debug.h>

void GameObjectSliderX::Update(float deltaTime)
{
    if(!myTransform)
    {
        myTransform = GetEntity()->GetComponent<Feng::Transform>();
        start = myTransform->GetPosition();
    }
    
    constexpr float speedFactor = 0.5f;
    float time = Feng::Engine::Time();
    float xOffset = std::sin(speedFactor * time) * amplitude;
    Feng::Vector3 position = start + xOffset * Feng::Vector3::OneX;
    myTransform->SetPosition(position);
}
