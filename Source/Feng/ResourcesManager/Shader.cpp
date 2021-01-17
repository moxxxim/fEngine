#include <Feng/ResourcesManager/Shader.h>

#include <Feng/ResourcesManager/ResourcesManager.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/Utils/Debug.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include <fstream>
#include <sstream>

namespace feng
{
    namespace SShader
    {
        std::string LoadShaderSource(const std::string& path)
        {
            std::ifstream shaderFile;
            shaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
            std::stringstream shaderStream;
            try
            {
                shaderFile.open(path);
                shaderStream << shaderFile.rdbuf();
                shaderFile.close();
            }
            catch (const std::ifstream::failure &e)
            {
                Debug::LogError("Failed to load shader file: " + path + ". Error: " + e.what());
                return "";
            }

            return shaderStream.str();
        }

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

            return static_cast<uint32_t>(shader);
        }
    }

    Shader::Shader(const std::string& vs, const std::string& fs)
        : program{UndefinedProgram}
    {
        Load(vs, fs);
    }

    Shader::~Shader()
    {
        Unload();
    }

    void Shader::StartUse() const
    {
        EnableState();
        glUseProgram(program);
    }

    void Shader::StopUse() const
    {
        DisableStates();
    }

    bool Shader::TryGetAttributeLocation(const char *name, uint32_t& location) const
    {
        if(auto it = attributes.find(name); it != attributes.end())
        {
            location = it->second;
            return true;
        }

        return false;
    }

    bool Shader::TryGetUniformLocation(const char *name, uint32_t& location) const
    {
        if(auto it = uniforms.find(name); it != uniforms.end())
        {
            location = it->second;
            return true;
        }

        return false;
    }

    bool Shader::HasUniform(const char *name) const
    {
        auto it = uniforms.find(name);
        return it != uniforms.end();
    }

    bool Shader::SetUniformBool(const char *name, bool value) const
    {
        SetUniformInt(name, static_cast<int32_t>(value));
    }

    bool Shader::SetUniformInt(const char *name, int32_t value) const
    {
        if(auto it = uniforms.find(name); it != uniforms.end())
        {
            glUniform1i(it->second, value);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformFloat(const char *name, float value) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1f(location, value);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector2(const char *name, const Vector2& value) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform2f(location, value.x, value.y);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector3(const char *name, const Vector3& value) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform3f(location, value.x, value.y, value.z);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector4(const char *name, const Vector4& value) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform4f(location, value.x, value.y, value.z, value.w);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformMatrix3(const char *name, const Matrix3& matrix) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, matrix.data);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformMatrix4(const char *name, const Matrix4& matrix) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformBuffer(const char *name, uint32_t index) const
    {
        uint32_t location = glGetUniformBlockIndex(program, name);
        if(location != UndefinedParamLocation)
        {
            glUniformBlockBinding(program, location, index);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformFloatArray(const char *name, const float *value, int size) const
    {
        uint32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1fv(location, size, value);
            return true;
        }

        return false;
    }

    void Shader::EnableState() const
    {
//        if ((int)m_cachedInfo.States & (int)ShaderStates::Culling)
//        {
//            glEnable(GL_CULL_FACE);
//        }
//
//        if ((int)m_cachedInfo.States & (int)ShaderStates::AlphaTest)
//        {
//            glEnable(GL_BLEND);
//            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
//        }
//
//        if ((int)m_cachedInfo.States & (int)ShaderStates::DepthTest)
//        {
//            glEnable(GL_DEPTH_TEST);
//        }
    }

    void Shader::DisableStates() const
    {
//        if ((int)m_cachedInfo.States & (int)feng::ShaderStates::Culling)
//        {
//            glDisable(GL_CULL_FACE);
//        }
//
//        if ((int)m_cachedInfo.States & (int)feng::ShaderStates::AlphaTest)
//        {
//            glDisable(GL_BLEND);
//        }
//
//        if ((int)m_cachedInfo.States & (int)feng::ShaderStates::DepthTest)
//        {
//            glDisable(GL_DEPTH_TEST);
//        }
    }

    void Shader::FetchActiveAttributes()
    {
        int attributesCount;
        glGetProgramiv(program, GL_ACTIVE_ATTRIBUTES, &attributesCount);

        for (int i = 0; i < attributesCount; ++i)
        {
            char name[80];
            GLint length = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveAttrib(program, i, 80, &length, &size, &type, name);
            int attributeLocation = glGetAttribLocation(program, name);
            if (attributeLocation != UndefinedParamLocation)
            {
                attributes[name] = attributeLocation;
            }
        }
    }

    void Shader::FetchActiveUniforms()
    {
        int uniformsCount;
        glGetProgramiv(program, GL_ACTIVE_UNIFORMS, &uniformsCount);

        for (int i = 0; i < uniformsCount; ++i)
        {
            char name[80];
            GLint length = 0;
            GLint size = 0;
            GLenum type = 0;
            glGetActiveUniform(program, i, 80, &length, &size, &type, name);
            int uniformLocation = glGetUniformLocation(program, name);
            if (uniformLocation != UndefinedParamLocation)
            {
                uniforms[name] = uniformLocation;
            }
        }
    }

    void Shader::Load(const std::string& vs, const std::string& fs)
    {
        uint32_t vertexShader = SShader::CompileShader(Shader::eType::Vertex, vs);
        if (vertexShader == UndefinedProgram)
        {
            return;
        }

        uint32_t fragmentShader = SShader::CompileShader(Shader::eType::Fragment, fs);
        if (fragmentShader == UndefinedProgram)
        {
            glDeleteShader(vertexShader);
            return;
        }

        program = glCreateProgram();
        if(program != UndefinedProgram)
        {
            glAttachShader(program, vertexShader);
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);

            GLint success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if(!success)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetProgramInfoLog(program, logSize, nullptr, errorLog);

                Debug::LogError("Failed to create shader program:");
                Debug::LogError(errorLog);

                glDeleteProgram(program);
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        if (program != UndefinedProgram)
        {
            FetchActiveAttributes();
            FetchActiveUniforms();
        }
    }

    void Shader::Unload()
    {
        glDeleteProgram(program);
    }

    std::unique_ptr<Shader> LoadShader(const std::string& vsPath, const std::string& fsPath)
    {
        std::string vSource = SShader::LoadShaderSource(vsPath);
        std::string fSource = SShader::LoadShaderSource(fsPath);
        return std::make_unique<Shader>(vSource, fSource);
    }
}
