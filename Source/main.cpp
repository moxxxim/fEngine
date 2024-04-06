#include <Feng/Core/Engine.hpp>
#include <Classes/TestSceneCreator.hpp>

#include <iostream>

int main(int, const char *)
{
    Feng::Engine engine;
    std::unique_ptr<Feng::Scene> scene = test::CreateTestScene();
    engine.SetScene(std::move(scene));
    engine.SetShadowsEnabled(true);

    return engine.Run();
}
