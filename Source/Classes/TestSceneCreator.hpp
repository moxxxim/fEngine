#pragma once

#include <Feng/ScenesManager/Scene.h>
#include <memory>

namespace test
{
    std::unique_ptr<feng::Scene> CreateTestScene();
}
