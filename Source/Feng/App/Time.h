#pragma once

namespace feng
{
    class ScenesManager;

    class Time final
    {
        friend ScenesManager;

    public:
        Time(const Time& other) = delete;
        Time(Time&& other) = delete;
        Time& operator = (const Time& other) = delete;
        Time& operator = (Time&& other) = delete;

        float GetTime() const;
        float GetSceneTime() const;
        float GetDeltaTime() const;
        static Time * GetInstance();

    private:
        Time() = default;
        ~Time() = default;

        float m_time;
        float m_sceneTime;
        float m_deltaTime;
    };

    inline float Time::GetTime() const
    {
        return m_time;
    }

    inline float Time::GetSceneTime() const
    {
        return m_sceneTime;
    }

    inline float Time::GetDeltaTime() const
    {
        return m_deltaTime;
    }
}
