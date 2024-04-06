#include <Classes/Scripts/CameraStabilizer.hpp>

#include <Feng/App/Globals.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/ScenesManager/Camera.h>
#include <Feng/ScenesManager/Entity.h>
#include <algorithm>

void CameraStabilizer::SetMinMaxFov(float min, float max)
{
    minFovY = min;
    maxFovY = max;
}

void CameraStabilizer::Update(float)
{
    if(!myCam)
    {
        Feng::Entity *myEntity = GetEntity();
        myCam = myEntity->GetComponent<Feng::Camera>();
        fovY = myCam->GetFovY();
    }

    fovY -= static_cast<float>(Feng::Engine::GetMouseScroll().coord.y);
    fovY = std::max(fovY, minFovY);
    fovY = std::min(fovY, maxFovY);
    fovY = std::clamp(fovY, minFovY, maxFovY);

    myCam->SetFovY(fovY);
}
