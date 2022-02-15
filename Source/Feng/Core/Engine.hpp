#pragma once

#include <Feng/Core/Input.hpp>
#include <Feng/Math/Vector2.h>
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

        Engine();
        int32_t Run();

    private:
        static void UpdateTime();
        static void UpdateInputKeys();

        static std::array<eKeyStatus, InputKey::eKey::Count> inputKeys;
        static float deltaTime;
        static float time;
        bool showDepth = false;
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
}
