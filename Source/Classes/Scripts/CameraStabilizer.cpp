#include <Classes/Scripts/CameraStabilizer.hpp>

#include <Feng/App/Globals.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>

void CameraStabilizer::Update(float deltaTime)
{
    if(!myCam)
    {
        Feng::Entity *myEntity = GetEntity();
        myCam = myEntity->GetComponent<Feng::Camera>();
    }
    
    fovY -= static_cast<float>(Feng::Engine::GetMouseScroll().y);
    fovY = std::max(fovY, 1.0f);
    fovY = std::min(fovY, 45.0f);

    myCam->SetFovY(fovY);
    myCam->SetAspectRatio(static_cast<float>(Feng::Screen::ScreenWidth)/Feng::Screen::ScreenHeight);
    myCam->SetNearClipPlane(0.1f);
    myCam->SetFarClipPlane(100.f);
}
