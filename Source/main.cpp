#include <Feng/Core/Engine.hpp>
#include <Classes/TestSceneCreator.hpp>

int main(int argc, const char * argv[])
{
    Feng::Engine engine;
    std::unique_ptr<Feng::Scene> scene = test::CreateTestScene();
    engine.SetScene(std::move(scene));
    
    return engine.Run();
} 
