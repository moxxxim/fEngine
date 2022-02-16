#include <Classes/Scripts/CameraWasdController.h>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

void CameraWasdController::Update(float deltaTime)
{
    if(!myTransform)
    {
        feng::Entity *myEntity = GetEntity();
        myTransform = myEntity->GetComponent<feng::Transform>();
    }

    UpdatePosition(deltaTime);
    UpdateRotation();
}

void CameraWasdController::UpdatePosition(float deltaTime)
{
    float speedMultiplier = 1;
    if(feng::Engine::IsKeyPressed(feng::InputKey::Kb_Left_Shift))
    {
        speedMultiplier = 2;
    }
    else if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Left_Control))
    {
        speedMultiplier = 0.5f;
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_W))
    {
        feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(speed * deltaTime * speedMultiplier * forward);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_S))
    {
        feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(-speed * deltaTime * speedMultiplier * forward);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_A))
    {
        feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(-speed * deltaTime * speedMultiplier * right);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_D))
    {
        feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(speed * deltaTime * speedMultiplier * right);
    }
}

void CameraWasdController::UpdateRotation()
{
    static bool firstMouse = true;
    
    feng::Vector2 mousePos = feng::Engine::GetMousePos();
    
    if (firstMouse)
    {
        lastMouseX = mousePos.x;
        lastMouseY = mousePos.y;
        firstMouse = false;
    }

    float xoffset = mousePos.x - lastMouseX;
    float yoffset = lastMouseY - mousePos.y;
    lastMouseX = mousePos.x;
    lastMouseY = mousePos.y;

    xoffset *= camSensitivity;
    yoffset *= camSensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
    {
        pitch =  89.0f;
    }

    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    myTransform->SetRotation(feng::mat3::MakeRotationY(yaw), feng::eSpace::World);
    myTransform->SetRotation(feng::mat3::MakeRotationX(pitch), feng::eSpace::Self);
}
