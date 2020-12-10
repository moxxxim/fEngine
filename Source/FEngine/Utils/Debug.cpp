#include <FEngine/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>
#include <iostream>
#include <vector>

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

    void Debug::PrintErrorOpenGL(const char *file, int line)
    {
        GLenum errorCode;
        while ((errorCode = glGetError()) != GL_NO_ERROR)
        {
            std::string error;
            switch (errorCode)
            {
                case GL_INVALID_ENUM:
                error = "INVALID_ENUM";
                break;

                case GL_INVALID_VALUE:
                error = "INVALID_VALUE";
                break;

                case GL_INVALID_OPERATION:
                error = "INVALID_OPERATION";
                break;

                case GL_STACK_OVERFLOW:
                error = "STACK_OVERFLOW";
                break;

                case GL_STACK_UNDERFLOW:
                error = "STACK_UNDERFLOW";
                break;

                case GL_OUT_OF_MEMORY:
                error = "OUT_OF_MEMORY";
                break;

                case GL_INVALID_FRAMEBUFFER_OPERATION:
                error = "INVALID_FRAMEBUFFER_OPERATION";
                break;
            }

            std::cout << error << " | " << file << " (" << line << ")" << "\n";
        }

        return errorCode;
    }

    void Debug::LogRenderInfoOpenGL()
    {
        GLint extensionsCount = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);

        std::vector<const GLubyte*> extensions;
        for(GLint i = 0; i < extensionsCount; ++i)
        {
            const GLubyte* extension = glGetStringi(GL_EXTENSIONS, i);
            extensions.push_back(extension);
        }

        int maxAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);

        const GLubyte *version = glGetString(GL_VERSION);
        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *renderer = glGetString(GL_RENDERER);

        std::cout << "OpenGL Version: " << version << "\n";
        std::cout << "OpenGL Vendor: " << vendor << "\n";
        std::cout << "OpenGL Renderer: " << renderer << "\n";
        std::cout << "OpenGL Maximum vertex attributes number: " << maxAttributes << "\n";
        std::cout << "OpenGL Supported Extensions:\n";
        for(const GLubyte* extension : extensions)
        {
            std::cout << extension << "\n";
        }

        std::cout << "\n";
    }
}
