#include <Feng/Core/Engine.hpp>
#include <Classes/TestSceneCreator.hpp>

#include <iostream>

int main(int argc, const char * argv[])
{
    Feng::Engine engine;
    std::unique_ptr<Feng::Scene> scene = test::CreateTestScene();
    engine.SetScene(std::move(scene));
    engine.SetShadowsEnabled(true);
    engine.SetDirectShowDebugShadowMap(false);

    return engine.Run();
}

