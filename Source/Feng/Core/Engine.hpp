#pragma once

#include <Feng/Core/Input.hpp>
#include <Feng/Math/Vector2.h>
#include <Feng/ScenesManager/Scene.h>
#include <array>
#include <cstdint>

namespace feng
{
    class Engine final
    {
    public:
        static float DeltaTime();
        static float Time();
        static bool IsKeyPressed(InputKey::eKey key);
        static Vector2 GetMousePos();
        static Vector2 GetMouseScroll();
        static bool IsShowDepth();
        static void ShowDepth(bool show);

        Engine();
        int32_t Run();
        void SetScene(std::unique_ptr<Scene>&& aScene);
        
    private:
        static void UpdateTime();
        static void UpdateInputKeys();
        
        void Update();
        void Render();

        static std::array<eKeyStatus, InputKey::eKey::Count> inputKeys;
        static float deltaTime;
        static float time;
        static bool showDepth;
        
        std::unique_ptr<feng::Scene> scene;
    };

    inline float Engine::DeltaTime()
    {
        return deltaTime;
    }
    
    inline float Engine::Time()
    {
        return time;
    }
    
    inline bool Engine::IsKeyPressed(InputKey::eKey key)
    {
        return inputKeys[static_cast<uint32_t>(key)] == eKeyStatus::Pressed;
    }
    
    inline bool Engine::IsShowDepth()
    {
        return showDepth;
    }
    
    inline void Engine::ShowDepth(bool show)
    {
        showDepth = show;
    }
    
    inline void Engine::SetScene(std::unique_ptr<Scene>&& aScene)
    {
        scene = std::move(aScene);
    }
}
