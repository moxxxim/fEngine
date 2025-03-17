#include <Feng/ResourcesManager/Shader.h>

#include <Feng/Core/FengGL.h>
#include <Feng/Utils/Render/RenderUtils.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <Feng/Utils/Debug.h>

#include <fstream>
#include <sstream>

namespace Feng
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

        uint32_t CompileShader(eShaderType type, const std::string& sourceCode)
        {
            GLenum glShaderType = Render::ToOpenGLValue(type);
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

    Shader::Shader(const std::string& vs, const std::string& fs, const std::string& gs /*= ""*/)
    {
        Load(vs, fs, gs);
    }

    Shader::~Shader()
    {
        Unload();
    }

    void Shader::Use() const
    {
        glUseProgram(program);
    }

    bool Shader::TryGetAttributeLocation(const char *name, int32_t& location) const
    {
        if(auto it = attributes.find(name); it != attributes.end())
        {
            location = it->second;
            return true;
        }

        return false;
    }

    bool Shader::TryGetUniformLocation(const char *name, int32_t& location) const
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
        return SetUniformInt(name, static_cast<int32_t>(value));
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
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1f(location, value);
            return true;
        }

        return false;
    }
    
    bool Shader::SetUniformFloats(const char *name, const std::vector<float>& value) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1fv(location, static_cast<int32_t>(value.size()), value.data());
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector2(const char *name, const Vector2& value) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform2f(location, value.coord.x, value.coord.y);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector3(const char *name, const Vector3& value) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform3f(location, value.coord.x, value.coord.y, value.coord.z);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformVector4(const char *name, const Vector4& value) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform4f(location, value.coord.x, value.coord.y, value.coord.z, value.coord.w);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformMatrix3(const char *name, const Matrix3& matrix) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniformMatrix3fv(location, 1, GL_FALSE, matrix.data);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformMatrix4(const char *name, const Matrix4& matrix) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, matrix.data);
            return true;
        }

        return false;
    }
    
    bool Shader::SetUniformMatrices4(const char *name, const std::vector<Matrix4>& matrices) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniformMatrix4fv(
                               location,
                               static_cast<int32_t>(matrices.size()),
                               GL_FALSE,
                               matrices.data()->data);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformBuffer(const char *name, uint32_t index) const
    {
        int32_t location = glGetUniformBlockIndex(program, name);
        if(location != UndefinedParamLocation)
        {
            glUniformBlockBinding(program, location, index);
            return true;
        }

        return false;
    }

    bool Shader::SetUniformFloatArray(const char *name, const float *value, int size) const
    {
        int32_t location = UndefinedParamLocation;
        if(TryGetUniformLocation(name, location))
        {
            glUniform1fv(location, size, value);
            return true;
        }

        return false;
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
        Print_Errors_OpengGL();

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

    void Shader::Load(const std::string& vs, const std::string& fs, const std::string& gs)
    {
        uint32_t vertexShader = SShader::CompileShader(eShaderType::Vertex, vs);
        Print_Errors_OpengGL();
        if (vertexShader == UndefinedProgram)
        {
            return;
        }

        uint32_t fragmentShader = SShader::CompileShader(eShaderType::Fragment, fs);
        Print_Errors_OpengGL();
        if (fragmentShader == UndefinedProgram)
        {
            glDeleteShader(vertexShader);
            return;
        }
        
        uint32_t geometryShader = UndefinedProgram;
        if(!gs.empty())
        {
            geometryShader = SShader::CompileShader(eShaderType::Geometry, gs);
            Print_Errors_OpengGL();
            if (geometryShader == UndefinedProgram)
            {
                glDeleteShader(vertexShader);
                glDeleteShader(fragmentShader);
                return;
            }
        }

        program = glCreateProgram();
        if(program != UndefinedProgram)
        {
            glAttachShader(program, vertexShader);
            if(geometryShader != UndefinedProgram)
            {
                glAttachShader(program, geometryShader);
            }
            glAttachShader(program, fragmentShader);
            glLinkProgram(program);

            GLint success;
            glGetProgramiv(program, GL_LINK_STATUS, &success);
            if(success == GL_FALSE)
            {
                constexpr int logSize = 512;
                char errorLog[logSize];
                glGetProgramInfoLog(program, logSize, nullptr, errorLog);

                Debug::LogError("Failed to create shader program:");
                Debug::LogError(errorLog);

                glDeleteProgram(program);
                program = UndefinedProgram;
            }
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);
        if(geometryShader != UndefinedProgram)
        {
            glDeleteShader(geometryShader);
        }

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

    std::unique_ptr<Shader> LoadShader(const std::string& vsPath, const std::string& fsPath, const std::string& gsPath)
    {
        std::string vSource = SShader::LoadShaderSource(vsPath);
        std::string fSource = SShader::LoadShaderSource(fsPath);
        std::string gSource;
        if(!gsPath.empty())
        {
             gSource = SShader::LoadShaderSource(gsPath);
        }
        
        return std::make_unique<Shader>(vSource, fSource, gSource);
    }
}
