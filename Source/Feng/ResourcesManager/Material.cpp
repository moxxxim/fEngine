#include <Feng/ResourcesManager/Material.h>

#include <Feng/ResourcesManager/ResourceManager.h>
#include <Feng/ResourcesManager/Shader.h>

namespace feng
{
    Material::Material(std::unique_ptr<Shader>&& aShader)
        : shader {std::move(aShader)}
    { }

    bool Material::HasTexture(const std::string& name) const
    {
        return textures.find(name) != textures.end();
    }

    bool Material::HasFloat(const std::string& name) const
    {
        return floats.find(name) != floats.end();
    }

    bool Material::HasVector4(const std::string& name) const
    {
        return vectors4.find(name) != vectors4.end();
    }

    const Texture * Material::GetTexture(const std::string& name) const
    {
        if(auto it = textures.find(name); it != textures.end())
        {
            return it->second;
        }

        return nullptr;
    }

    void Material::SetTexture(const std::string& name, Texture *texture)
    {
        textures[name] = texture;
    }

    bool Material::TryGetFloat(const std::string& name, float& value) const
    {
        if (auto it = floats.find(name); it != floats.end())
        {
            value = it->second;
            return true;
        }

        return false;
    }

    void Material::SetFloat(const std::string& name, float value)
    {
        floats[name] = value;
    }

    bool Material::TryGetVector4(const std::string& name, Vector4& value) const
    {
        if (auto it = vectors4.find(name); it != vectors4.end())
        {
            value = it->second;
            return true;
        }

        return false;
    }

    void Material::SetVector4(const std::string& name, const Vector4& value)
    {
        vectors4[name] = value;
    }
}
