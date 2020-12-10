#pragma once

#include <FEngine/Math/Vector2.h>
#include <FEngine/Math/Vector3.h>
#include <FEngine/Math/Vector4.h>

#include <map>
#include <utility>

namespace feng
{
    class Texture;
    class Shader;

    class Material final
    {
    public:
        explicit Material(std::unique_ptr<Shader>&& aShader);

        bool HasTexture(const std::string& name) const;
        bool HasFloat(const std::string& name) const;
        bool HasVector4(const std::string& name) const;

        Shader* GetShader() const;
        void SetShader(std::unique_ptr<Shader>&& aShader);

        const Texture * GetTexture(const std::string& name) const;
        void SetTexture(const std::string& name, Texture *texture);

        bool TryGetFloat(const std::string& name, float& value) const;
        void SetFloat(const std::string& name, float value);

        bool TryGetVector4(const std::string& name, Vector4& value) const;
        void SetVector4(const std::string& name, const Vector4& value);

    private:
        std::unique_ptr<Shader> shader;
        std::map<std::string, Texture*> textures;
        std::map<std::string, float> floats;
        std::map<std::string, int> ints;
        std::map<std::string, Vector2> vectors2;
        std::map<std::string, Vector3> vectors3;
        std::map<std::string, Vector4> vectors4;
    };

    inline Shader* Material::GetShader() const
    {
        return shader.get();
    }

    inline void Material::SetShader(std::unique_ptr<Shader>&& aShader)
    {
        shader = std::move(aShader);
    }
}
