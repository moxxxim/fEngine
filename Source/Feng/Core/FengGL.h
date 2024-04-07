#pragma once

#ifdef WIN32
    #include <glad/glad.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/gl3.h>
#endif

#include <Glfw/glfw3.h>

namespace Feng
{
    bool TryInitOpenGL();
}
