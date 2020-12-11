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
        static void PrintErrorOpenGL(const char *file, int line);
        static void LogRenderInfoOpenGL();
    };
}

#define Print_Errors_OpengGL() feng::Debug::PrintErrorOpenGL(__FILE__, __LINE__)
