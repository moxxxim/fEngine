#pragma once

#include <FEngine/App/Object.h>
#include <map>

namespace feng
{
    class Texture;
    class Shader;

    // TODO: m.alekseev remove this forwards.
    class Vector2;
    class Vector3;
    class Vector4;

    class Material final : public Object
    {
    public:
        explicit Material(Shader *shader);

        bool HasTexture(const char *name) const;
        bool HasFloat(const char *name) const;
        bool HasVector4(const char *name) const;

        Shader * GetShader() const;
        const Texture * GetTexture(const char *name) const;
        float GetFloat(const char *name) const;
        Vector4 GetVector4(const char *name) const;

        void SetShader(Shader *shader);
        void SetTexture(const char *name, Texture *texture);
        void SetFloat(const char *name, float value);
        void SetVector4(const char *name, const Vector4& value);

        void Destroy() override;

    private:
        Shader *m_shader = nullptr;
        std::map<const char*, Texture*> m_textures;
        std::map<const char*, float> m_floats;
        std::map<const char*, int> m_ints;
        std::map<const char*, Vector2> m_vectors2;
        std::map<const char*, Vector3> m_vectors3;
        std::map<const char*, Vector4> m_vectors4;
    };

    Material::Material(Shader *shader) :
        Object(ObjectType::Resource),
        m_shader(shader)
    {
    }

    inline feng::Shader * Material::GetShader() const
    {
        return m_shader;
    }

    inline void Material::SetShader(Shader *shader)
    {
        m_shader = shader;
    }
}
