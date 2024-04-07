#include <Feng/Core/Engine.hpp>
#include <Classes/TestSceneCreator.hpp>

#include <iostream>

#include <Feng/Utils/Debug.h>

#ifdef WIN32
#include <Windows.h>
#endif

namespace SMain
{
    void LogWorkingDir()
    {
#ifdef WIN32
        char buffer[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, buffer);
        Feng::Debug::LogMessage("[Feng] Start in directory: " + std::string{ buffer });
#endif
    }
}

int main(int, const char **)
{
    SMain::LogWorkingDir();

    Feng::Engine engine;
    std::unique_ptr<Feng::Scene> scene = test::CreateTestScene();
    engine.SetScene(std::move(scene));
    engine.SetShadowsEnabled(true);

    return engine.Run();
}
