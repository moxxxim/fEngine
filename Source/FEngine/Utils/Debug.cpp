#include <FEngine/Utils/Debug.h>

#include <iostream>

namespace fengine
{
    void Debug::LogMessage(const char *message)
    {
        std::cout << message << "\n";
    }

    void Debug::LogWarning(const char *message)
    {
        Debug::LogMessage(message);
    }

    void Debug::LogError(const char *message)
    {
        Debug::LogMessage(message);
    }
}
