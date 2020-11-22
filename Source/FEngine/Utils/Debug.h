#pragma once

namespace fengine
{
    class Debug final
    {
    public:
        static void LogMessage(const char *message);
        static void LogWarning(const char *message);
        static void LogError(const char *message);
    };
}
