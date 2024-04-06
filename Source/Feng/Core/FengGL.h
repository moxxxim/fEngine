#pragma once

#ifdef WIN32
    #define NOMINMAX
    #include <glad/glad.h>
#elif defined(__APPLE__)
    #include <OpenGL/gl.h>
    #include <OpenGL/gl3.h>
#endif

#include <GLFW/glfw3.h>