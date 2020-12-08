#pragma once

#include <string>

namespace feng
{
    class Debug final
    {
    public:
        static void LogMessage(const std::string &message);
        static void LogWarning(const std::string &message);
        static void LogError(const std::string &message);
    };
}
