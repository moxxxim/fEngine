#include <Feng/Core/FengGL.h>

#include <Feng/Utils/Debug.h>

namespace Feng
{
    bool TryInitOpenGL()
    {
#ifdef WIN32
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            Feng::Debug::LogError("Failed to initialize GLAD");
            return false;
        }
#endif

        return true;
    }
}