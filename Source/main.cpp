#include <Feng/Core/Engine.hpp>
#include <Classes/TestSceneCreator.hpp>

int main(int argc, const char * argv[])
{
    feng::Engine engine;
    std::unique_ptr<feng::Scene> scene = test::CreateTestScene();
    engine.SetScene(std::move(scene));
    
    return engine.Run();
} 
