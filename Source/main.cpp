#include <Classes/TestSceneCreator.hpp>
#include <Feng/Core/Engine.hpp>
#include <Feng/Utils/Debug.h>

#include <iostream>

#ifdef WIN32
#include <Windows.h>
#elif defined(__APPLE__)
#include <filesystem>
#endif

namespace SMain
{
    void LogWorkingDir()
    {
        std::string working_dir;
#ifdef WIN32
        char buffer[MAX_PATH];
        ::GetCurrentDirectory(MAX_PATH, buffer);
        working_dir = std::string{ buffer };
#elif defined(__APPLE__)
        working_dir = std::filesystem::current_path().string();
#endif
        Feng::Debug::LogMessage("[Feng] Start in directory: " + working_dir);
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
