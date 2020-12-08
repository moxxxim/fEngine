#include <FEngine/Utils/Debug.h>

#include <iostream>

namespace feng
{
    void Debug::LogMessage(const std::string &message)
    {
        std::cout << message << "\n";
    }

    void Debug::LogWarning(const std::string &message)
    {
        Debug::LogMessage(message);
    }

    void Debug::LogError(const std::string &message)
    {
        Debug::LogMessage(message);
    }
}
