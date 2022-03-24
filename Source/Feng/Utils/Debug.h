#pragma once

#include <string>

namespace Feng
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

#define Print_Errors_OpengGL() Feng::Debug::PrintErrorOpenGL(__FILE__, __LINE__)

#define ASSERTIONS_ENABLED
#if defined ASSERTIONS_ENABLED
/*
 Define some inline assembly that causes a break into the debugger.
 This will be different on each target CPU.
 */
#define debugBreak() asm { int 3 }

// check the expression and fail if it is false
#define FASSERT(expr)                                        \
if (expr) { }                                                \
else                                                         \
{                                                            \
    Feng::Debug::LogError(#expr + __FILE__ + __LINE__);      \
    debugBreak();                                            \
}                                                           
#else
#define ASSERT(expr) // evaluates to nothing
#endif
