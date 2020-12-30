#pragma once

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>

#include <map>
#include <string>

namespace feng
{
    enum class ShaderStates : int;

    class Shader final
    {
    public:
        enum class eType : uint8_t
        {
            Vertex = 0,
            Fragment = 1,
        };

        Shader(const std::string& vs, const std::string& fs);
        ~Shader();

        bool IsLoaded() const;
        void StartUse() const;
        void StopUse() const;

        const std::map<std::string, int32_t> &GetAttributes() const;
        const std::map<std::string, int32_t> &GetUniforms() const;

        bool TryGetAttributeLocation(const char *name, uint32_t& location) const;
        bool TryGetUniformLocation(const char *name, uint32_t& location) const;
        bool HasUniform(const char *name) const;

        bool SetUniformBool(const char *name, bool value) const;
        bool SetUniformInt(const char *name, int32_t value) const;
        bool SetUniformFloat(const char *name, float value) const;
        bool SetUniformVector2(const char *name, const Vector2& value) const;
        bool SetUniformVector3(const char *name, const Vector3& value) const;
        bool SetUniformVector4(const char *name, const Vector4& value) const;
        bool SetUniformMatrix3(const char *name, const Matrix3& matrix) const;
        bool SetUniformMatrix4(const char *name, const Matrix4& matrix) const;

    private:
        bool SetUniformFloatArray(const char *name, const float *value, int size) const;
        void EnableState() const;
        void DisableStates() const;
        void FetchActiveAttributes();
        void FetchActiveUniforms();
        void Load(const std::string& vs, const std::string& fs);
        void Unload();

        static const int32_t UndefinedProgram = 0;
        static const int32_t UndefinedParamLocation = -1;

        std::map<std::string, int32_t> attributes;
        std::map<std::string, int32_t> uniforms;
        uint32_t program = Shader::UndefinedProgram;
    };

    std::unique_ptr<Shader> LoadShader(const std::string& vsPath, const std::string& fsPath);
}

namespace feng
{
    inline bool Shader::IsLoaded() const
    {
        return program != Shader::UndefinedProgram;
    }

    inline const std::map<std::string, int32_t> &Shader::GetAttributes() const
    {
        return attributes;
    }

    inline const std::map<std::string, int32_t> &Shader::GetUniforms() const
    {
        return uniforms;
    }
}
