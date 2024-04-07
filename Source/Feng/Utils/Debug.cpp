#include <Feng/Utils/Debug.h>

#include <Feng/Core/FengGL.h>
#include <iostream>
#include <vector>

#ifdef WIN32
    #include <Windows.h>
#endif

static_assert(sizeof(const char) == sizeof(const GLubyte));

namespace Feng
{
    void Debug::LogMessage(const std::string &message)
    {
#ifdef WIN32
        OutputDebugString(message.c_str());
        OutputDebugString("\n");
#endif
        std::cout << message << "\n";
    }

    void Debug::LogWarning(const std::string &message)
    {
        Debug::LogMessage("[warning] - " + message);
    }

    void Debug::LogError(const std::string &message)
    {
        Debug::LogMessage("[error] - " + message);
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

            LogMessage(error + " | " + file + " (" + std::to_string(line) + ") ");
        }
    }

    void Debug::LogRenderInfoOpenGL()
    {
        GLint extensionsCount = 0;
        glGetIntegerv(GL_NUM_EXTENSIONS, &extensionsCount);

        std::vector<std::string> extensions;
        for(GLint i = 0; i < extensionsCount; ++i)
        {
            const GLubyte* extension = glGetStringi(GL_EXTENSIONS, i);
            extensions.push_back(std::string{ reinterpret_cast<const char*>(extension) });
        }

        int maxAttributes;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &maxAttributes);

        const GLubyte *version = glGetString(GL_VERSION);
        const GLubyte *vendor = glGetString(GL_VENDOR);
        const GLubyte *renderer = glGetString(GL_RENDERER);
        const GLubyte *languageVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        LogMessage("------------------ Open GL Info ----------------------");
        LogMessage("OpenGL Version: " + std::string{ reinterpret_cast<const char*>(version) });
        LogMessage("OpenGL Vendor: " + std::string{ reinterpret_cast<const char*>(vendor) });
        LogMessage("OpenGL Renderer: " + std::string{ reinterpret_cast<const char*>(renderer) });
        LogMessage("OpenGL Language Version: " + std::string{ reinterpret_cast<const char*>(languageVersion) });
        LogMessage("OpenGL Maximum vertex attributes number: " + std::to_string(maxAttributes));
        LogMessage("OpenGL Supported Extensions:\n");
        for(const std::string& extension : extensions)
        {
            LogMessage(" -- " + extension);
        }

        LogMessage("------------------------------------------------------");
    }
}
