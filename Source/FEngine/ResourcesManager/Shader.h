#pragma once

//#include <FEngine/App/Object.h>
#include <FEngine/ResourcesManager/CachedShader.h>

#include <map>
#include <string>

namespace fengine
{
    enum class ShaderStates : int;

    class Shader final // : public Object
    {
    public:
        enum class eType : uint8_t
        {
            Vertex = 0,
            Fragment = 1,
        };

        Shader(const CachedShader& cachedShader);
        ~Shader();

        void Load();
        void Unload();
        void StartUse() const;
        void StopUse() const;
        //void Destroy() override;

        const std::map<std::string, int32_t> &GetAttributes() const;
        const std::map<std::string, int32_t> &GetUniforms() const;

        bool TryGetAttributeLocation(const char *name, uint32_t& location) const;
        bool TryGetUniformLocation(const char *name, uint32_t& location) const;
        bool HasUniform(const char *name) const;

        bool SetUniformBool(const char *name, bool value);
        bool SetUniformInt(const char *name, int32_t value);
        bool SetUniformFloat(const char *name, float value);
        bool SetUniformFloatArray(const char *name, const float *value, int size);

    private:
        void EnableState() const;
        void DisableStates() const;
        void FetchActiveAttributes();
        void FetchActiveUniforms();

        static const int32_t k_undefinedShaderParamLocation = -1;

        uint32_t m_program = 0;
        std::map<std::string, int32_t> m_attributes;
        std::map<std::string, int32_t> m_uniforms;
        CachedShader m_cachedInfo;
    };

    std::unique_ptr<Shader> LoadShader(const std::string& vsPath, const std::string& fsPath);
}

namespace fengine
{
    inline const std::map<std::string, int32_t> &Shader::GetAttributes() const
    {
        return m_attributes;
    }

    inline const std::map<std::string, int32_t> &Shader::GetUniforms() const
    {
        return m_uniforms;
    }
}
