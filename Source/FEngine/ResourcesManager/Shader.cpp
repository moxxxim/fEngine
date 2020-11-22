#include <FEngine/ResourcesManager/Shader.h>

#include <FEngine/ResourcesManager/CachedShader.h>
#include <FEngine/ResourcesManager/ResourcesManager.h>
#include <FEngine/ResourcesManager/ShaderParameters.h>
#include <FEngine/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include <fstream>
#include <sstream>

namespace fengine
{
    namespace SShader
    {
        uint32_t CompileShader(Shader::eType type, const std::string& sourceCode)
        {
            int glShaderType = (type == Shader::eType::Vertex)
            ? GL_VERTEX_SHADER
            : GL_FRAGMENT_SHADER;
            GLuint shader = glCreateShader(glShaderType);
            if(shader != 0)
            {
                const char *source = sourceCode.c_str();
                glShaderSource(shader, 1, &source, nullptr);
                glCompileShader(shader);

                // TODO: m.alekseev log errors only on certain build config.
                GLint success;
                glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
                if(!success)
                {
                    constexpr int logSize = 512;
                    char errorLog[logSize];
                    glGetShaderInfoLog(shader, logSize, nullptr, errorLog);

                    Debug::LogError("Failed to compile shader:");
                    Debug::LogError(errorLog);

                    glDeleteShader(shader);
                    shader = 0;
                }
            }

            return shader;
        }
    }

    Shader::Shader(const CachedShader& cachedShader) :
        //Object(ObjectType::Resource),
        m_program(k_undefinedShaderParamLocation),
        m_cachedInfo(cachedShader)
    {
        Load();
    }

    Shader::~Shader()
    {
        Unload();
    }

    void Shader::Load()
    {
        uint32_t vertexShader = SShader::CompileShader(Shader::eType::Vertex, m_cachedInfo.Vs);
        if (vertexShader == 0)
        {
            return;
        }

        uint32_t fragmentShader = SShader::CompileShader(Shader::eType::Fragment, m_cachedInfo.Fs);
        if (fragmentShader == 0)
        {
            glDeleteShader(vertexShader);
            return;
        }

        m_program = glCreateProgram();
        if(m_program != 0)
        {
            glAttachShader(m_program, vertexShader);
            glAttachShader(m_program, fragmentShader);
            glLinkProgram(m_program);

            GLint success;
            glGetProgramiv(m_program, GL_LINK_STATUS, &success);
            if(!success)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetProgramInfoLog(m_program, logSize, nullptr, errorLog);

                Debug::LogError("Failed to create shader program:");
                Debug::LogError(errorLog);

                glDeleteProgram(m_program);
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (m_program != 0)
        {
            FetchActiveAttributes();
            FetchActiveUniforms();
        }
    }

    void Shader::Unload()
    {
        glDeleteProgram(m_program);
    }

    void Shader::StartUse() const
    {
        EnableState();
        glUseProgram(m_program);
    }

    void Shader::StopUse() const
    {
        DisableStates();
    }

    //    void Shader::Destroy()
    //    {
    //        ResourcesManager *manager = ResourcesManager::GetInstance();
    //        manager->DestroyResource(GetId());
    //    }

    bool Shader::TryGetAttributeLocation(const char *name, uint32_t& location) const
    {
        if(auto it = m_attributes.find(name); it != m_attributes.end())
        {
            location = it->second;
            return true;
        }

        return false;
    }

    bool Shader::TryGetUniformLocation(const char *name, uint32_t& location) const
    {
        if(auto it = m_uniforms.find(name); it != m_uniforms.end())
        {
            location = it->second;
            return true;
        }

        return false;
    }

    bool Shader::HasUniform(const char *name) const
    {
        auto it = m_uniforms.find(name);
        return it != m_uniforms.end();
    }

    bool Shader::SetUniformBool(const char *name, bool value)
    {
        SetUniformInt(name, static_cast<int32_t>(value));
    }

    bool Shader::SetUniformInt(const char *name, int32_t value)
    {
        if(auto it = m_uniforms.find(name); it != m_uniforms.end())
        {
            glUniform1i(it->second, value);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformFloat(const char *name, float value)
    {
        uint32_t location = k_undefinedShaderParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1f(location, value);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformFloatArray(const char *name, const float *value, int size)
    {
        uint32_t location = k_undefinedShaderParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1fv(location, size, value);
            return true;
        }

        return false;
    }

    void Shader::EnableState() const
    {
        if ((int)m_cachedInfo.States & (int)ShaderStates::Culling)
        {
            glEnable(GL_CULL_FACE);
        }

        if ((int)m_cachedInfo.States & (int)ShaderStates::AlphaTest)
        {
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        }

        if ((int)m_cachedInfo.States & (int)ShaderStates::DepthTest)
        {
            glEnable(GL_DEPTH_TEST);
        }
    }

    void Shader::DisableStates() const
    {
        if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::Culling)
        {
            glDisable(GL_CULL_FACE);
        }

        if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::AlphaTest)
        {
            glDisable(GL_BLEND);
        }

        if ((int)m_cachedInfo.States & (int)fengine::ShaderStates::DepthTest)
        {
            glDisable(GL_DEPTH_TEST);
        }
    }

    void Shader::FetchActiveAttributes()
    {
        int attributesCount;
        glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &attributesCount);

        for (int i = 0; i < attributesCount; ++i)
        {
            char name[80];
            GLint length = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveAttrib(m_program, i, 80, &length, &size, &type, name);
            int attributeLocation = glGetAttribLocation(m_program, name);
            if (attributeLocation != k_undefinedShaderParamLocation)
            {
                m_attributes[name] = attributeLocation;
            }
        }
    }

    void Shader::FetchActiveUniforms()
    {
        int uniformsCount;
        glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &uniformsCount);

        for (int i = 0; i < uniformsCount; ++i)
        {
            char name[80];
            GLint length = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveUniform(m_program, i, 80, &length, &size, &type, name);
            int uniformLocation = glGetUniformLocation(m_program, name);
            if (uniformLocation != k_undefinedShaderParamLocation)
            {
                m_uniforms[name] = uniformLocation;
            }
        }
    }

    std::unique_ptr<Shader> LoadShader(const std::string& vsPath, const std::string& fsPath)
    {
        CachedShader cachedShader;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try
        {
            // open files
            vShaderFile.open(vsPath);
            fShaderFile.open(fsPath);

            std::stringstream vShaderStream;
            std::stringstream fShaderStream;

            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();

            // close file handlers
            vShaderFile.close();
            fShaderFile.close();

            cachedShader.Vs = vShaderStream.str();
            cachedShader.Fs = fShaderStream.str();
        }
        catch(const std::ifstream::failure &e)
        {
            Debug::LogError("Failed to load shader file: " + vsPath + ", " + fsPath + ". Error: " + e.what());
        }

        return std::make_unique<Shader>(cachedShader);
    }
}
