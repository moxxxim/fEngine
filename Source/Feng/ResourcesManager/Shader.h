#pragma once

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>

#include <cstdint>
#include <map>
#include <memory>
#include <string>
#include <vector>

namespace Feng
{
    enum class eShaderType : uint8_t
    {
        Vertex = 0,
        Fragment = 1,
        Geometry = 2,
    };

    class Shader final
    {
    public:
        Shader(const std::string& vs, const std::string& fs, const std::string& gs = "");
        ~Shader();

        bool IsLoaded() const;
        void Use() const;

        const std::map<std::string, int32_t>& GetAttributes() const;
        const std::map<std::string, int32_t>& GetUniforms() const;

        bool TryGetAttributeLocation(const char *name, int32_t& location) const;
        bool TryGetUniformLocation(const char *name, int32_t& location) const;
        bool HasUniform(const char *name) const;

        bool SetUniformBool(const char *name, bool value) const;
        bool SetUniformInt(const char *name, int32_t value) const;
        bool SetUniformFloat(const char *name, float value) const;
        bool SetUniformFloats(const char *name, const std::vector<float>& value) const;
        bool SetUniformVector2(const char *name, const Vector2& value) const;
        bool SetUniformVector3(const char *name, const Vector3& value) const;
        bool SetUniformVector4(const char *name, const Vector4& value) const;
        bool SetUniformMatrix3(const char *name, const Matrix3& matrix) const;
        bool SetUniformMatrix4(const char *name, const Matrix4& matrix) const;
        bool SetUniformMatrices4(const char *name, const std::vector<Matrix4>& matrices) const;
        bool SetUniformBuffer(const char *name, uint32_t index) const;

    private:
        bool SetUniformFloatArray(const char *name, const float *value, int size) const;
        void FetchActiveAttributes();
        void FetchActiveUniforms();
        void Load(const std::string& vs, const std::string& fs, const std::string& gs);
        void Unload();

        static const int32_t UndefinedProgram = 0;
        static const int32_t UndefinedParamLocation = -1;

        std::map<std::string, int32_t> attributes;
        std::map<std::string, int32_t> uniforms;
        uint32_t program = Shader::UndefinedProgram;
    };

    std::unique_ptr<Shader> LoadShader(
                                       const std::string& vsPath,
                                       const std::string& fsPath,
                                       const std::string& gsPath = "");
}

namespace Feng
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
