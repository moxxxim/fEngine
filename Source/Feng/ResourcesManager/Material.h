#pragma once

#include <Feng/Math/Vector2.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>
#include <Feng/Render/RenderBase.h>
#include <Feng/ResourcesManager/Shader.h>

#include <map>
#include <utility>

namespace Feng
{
    class Texture;

    class Material final
    {
    public:
        explicit Material(std::unique_ptr<Shader>&& aShader);
        ~Material();

        bool HasShader() const;
        bool HasTexture(const std::string& name) const;
        bool HasFloat(const std::string& name) const;
        bool HasVector4(const std::string& name) const;

        const Shader* GetShader() const;
        void SetShader(std::unique_ptr<Shader>&& aShader);

        const std::map<std::string, float>& GetFloats() const;
        bool TryGetFloat(const std::string& name, float& value) const;
        void SetFloat(const std::string& name, float value);

        const std::map<std::string, int>& GetInts() const;
        bool TryGetInt(const std::string& name, int& value) const;
        void SetInt(const std::string& name, int value);

        const std::map<std::string, Vector2>& GetVectors2() const;
        bool TryGetVector2(const std::string& name, Vector2& value) const;
        void SetVector2(const std::string& name, Vector2 value);

        const std::map<std::string, Vector3>& GetVectors3() const;
        bool TryGetVector3(const std::string& name, Vector3& value) const;
        void SetVector3(const std::string& name, Vector3 value);

        const std::map<std::string, Vector4>& GetVectors4() const;
        bool TryGetVector4(const std::string& name, Vector4& value) const;
        void SetVector4(const std::string& name, const Vector4& value);

        const std::map<std::string, const Texture*>& GetTextures() const;
        Texture* GetTexture(const std::string& name);
        const Texture* GetTexture(const std::string& name) const;
        void SetTexture(const std::string& name, const Texture *texture);

        bool IsTransparent() const;
        void SetTransparent(bool aTransparent);
        void SetDrawFace(eDrawFace face);
        void Apply();

    private:
        template <typename T>
        static bool TryGetValue(const std::map<std::string, T>& values, const std::string& name, T& value);

        void ApplyStates();
        std::unique_ptr<Shader> shader;
        std::map<std::string, float> floats;
        std::map<std::string, int> ints;
        std::map<std::string, Vector2> vectors2;
        std::map<std::string, Vector3> vectors3;
        std::map<std::string, Vector4> vectors4;
        std::map<std::string, const Texture*> textures;
        eDrawFace drawFace = eDrawFace::Cw;
        bool transparent = false;
    };

    inline const Shader* Material::GetShader() const
    {
        return shader.get();
    }

    inline void Material::SetShader(std::unique_ptr<Shader>&& aShader)
    {
        shader = std::move(aShader);
    }

    inline const std::map<std::string, float>& Material::GetFloats() const
    {
        return floats;
    }

    inline bool Material::TryGetFloat(const std::string& name, float& value) const
    {
        return TryGetValue<float>(floats, name, value);
    }

    inline const std::map<std::string, int>& Material::GetInts() const
    {
        return ints;
    }

    inline bool Material::TryGetInt(const std::string& name, int& value) const
    {
        return TryGetValue<int>(ints, name, value);
    }

    inline const std::map<std::string, Vector2>& Material::GetVectors2() const
    {
        return vectors2;
    }

    inline bool Material::TryGetVector2(const std::string& name, Vector2& value) const
    {
        return TryGetValue<Vector2>(vectors2, name, value);
    }

    inline const std::map<std::string, Vector3>& Material::GetVectors3() const
    {
        return vectors3;
    }

    inline bool Material::TryGetVector3(const std::string& name, Vector3& value) const
    {
        return TryGetValue<Vector3>(vectors3, name, value);
    }

    inline const std::map<std::string, Vector4>& Material::GetVectors4() const
    {
        return vectors4;
    }

    inline bool Material::TryGetVector4(const std::string& name, Vector4& value) const
    {
        return TryGetValue<Vector4>(vectors4, name, value);
    }

    inline const std::map<std::string, const Texture*>& Material::GetTextures() const
    {
        return textures;
    }

    template <typename T>
    bool Material::TryGetValue(const std::map<std::string, T>& values, const std::string& name, T& value)
    {
        if (auto it = values.find(name); it != values.end())
        {
            value = it->second;
            return true;
        }

        return false;
    }
}
