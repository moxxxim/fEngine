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
    UpdateRotationArrows(deltaTime);
    UpdateRotationMouse();
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
        const feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(speed * deltaTime * speedMultiplier * forward);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_S))
    {
        const feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(-speed * deltaTime * speedMultiplier * forward);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_A))
    {
        const feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(-speed * deltaTime * speedMultiplier * right);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_D))
    {
        const feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(speed * deltaTime * speedMultiplier * right);
    }
    
    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Q))
    {
        const feng::Vector3 up = feng::Vector3::OneY;
        myTransform->Move(-speed * deltaTime * speedMultiplier * up);
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_E))
    {
        const feng::Vector3 up = feng::Vector3::OneY;
        myTransform->Move(speed * deltaTime * speedMultiplier * up);
    }
}

void CameraWasdController::UpdateRotationArrows(float deltaTime)
{
    static bool firstMouse = true;
    
    float xoffset = 0;
    float yoffset = 0;
    
    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Up))
    {
        yoffset = deltaTime;
    }

    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Down))
    {
        yoffset = -deltaTime;
    }
    
    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Right))
    {
        xoffset = -deltaTime;
    }
    
    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_Left))
    {
        xoffset = deltaTime;
    }

    yaw += 100 * xoffset;
    pitch += 100 * yoffset;

    if(pitch > 89.0f)
    {
        pitch =  89.0f;
    }

    if(pitch < -89.0f)
    {
        pitch = -89.0f;
    }

    myTransform->SetRotation(feng::Quaternion{feng::Vector3::OneY, yaw}, feng::eSpace::World);
    myTransform->SetRotation(feng::Quaternion{feng::Vector3::OneX, pitch}, feng::eSpace::Self);
}

void CameraWasdController::UpdateRotationMouse()
{
    static bool firstMouse = true;
    
    feng::Vector2 mousePos = feng::Engine::GetMousePos();
    
    if (firstMouse)
    {
        lastMouseX = mousePos.x;
        lastMouseY = mousePos.y;
        firstMouse = false;
    }

    float xoffset = -(mousePos.x - lastMouseX);
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

    myTransform->SetRotation(feng::Quaternion{feng::Vector3::OneY, yaw}, feng::eSpace::World);
    myTransform->SetRotation(feng::Quaternion{feng::Vector3::OneX, pitch}, feng::eSpace::Self);
}
