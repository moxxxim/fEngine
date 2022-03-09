#include <Classes/Scripts/CameraWasdController.h>

#include <Feng/Core/Engine.hpp>
#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

void CameraWasdController::Update(float deltaTime)
{
    if(!myTransform)
    {
        Feng::Entity *myEntity = GetEntity();
        myTransform = myEntity->GetComponent<Feng::Transform>();
    }

    UpdatePosition(deltaTime);
    UpdateRotationArrows(deltaTime);
    UpdateRotationMouse();
}

void CameraWasdController::UpdatePosition(float deltaTime)
{
    float speedMultiplier = 1;
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Left_Shift))
    {
        speedMultiplier = 2;
    }
    else if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Left_Control))
    {
        speedMultiplier = 0.5f;
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_W))
    {
        const Feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(speed * deltaTime * speedMultiplier * forward);
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_S))
    {
        const Feng::Vector3 forward = myTransform->GetForward();
        myTransform->Move(-speed * deltaTime * speedMultiplier * forward);
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_A))
    {
        const Feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(-speed * deltaTime * speedMultiplier * right);
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_D))
    {
        const Feng::Vector3 right = myTransform->GetRight();
        myTransform->Move(speed * deltaTime * speedMultiplier * right);
    }
    
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Q))
    {
        const Feng::Vector3 up = Feng::Vector3::OneY;
        myTransform->Move(-speed * deltaTime * speedMultiplier * up);
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_E))
    {
        const Feng::Vector3 up = Feng::Vector3::OneY;
        myTransform->Move(speed * deltaTime * speedMultiplier * up);
    }
}

void CameraWasdController::UpdateRotationArrows(float deltaTime)
{
    static bool firstMouse = true;
    
    float xoffset = 0;
    float yoffset = 0;
    
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Up))
    {
        yoffset = deltaTime;
    }

    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Down))
    {
        yoffset = -deltaTime;
    }
    
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Right))
    {
        xoffset = -deltaTime;
    }
    
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_Left))
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

    myTransform->SetRotation(Feng::Quaternion{Feng::Vector3::OneY, yaw}, Feng::eSpace::World);
    myTransform->SetRotation(Feng::Quaternion{Feng::Vector3::OneX, pitch}, Feng::eSpace::Self);
}

void CameraWasdController::UpdateRotationMouse()
{
    static bool firstMouse = true;
    
    Feng::Vector2 mousePos = Feng::Engine::GetMousePos();
    
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

    myTransform->SetRotation(Feng::Quaternion{Feng::Vector3::OneY, yaw}, Feng::eSpace::World);
    myTransform->SetRotation(Feng::Quaternion{Feng::Vector3::OneX, pitch}, Feng::eSpace::Self);
}
