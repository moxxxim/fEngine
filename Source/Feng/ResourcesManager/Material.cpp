#include <Feng/ResourcesManager/Material.h>

#include <Feng/ResourcesManager/ResourceManager.h>
#include <Feng/ResourcesManager/Shader.h>

namespace Feng
{
    Material::Material(std::unique_ptr<Shader>&& aShader)
        : shader {std::move(aShader)}
    { }

    Material::~Material() = default;

    bool Material::HasShader() const
    {
        if(shader != nullptr)
        {
            return shader->IsLoaded();
        }

        return false;
    }

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

    void Material::SetFloat(const std::string& name, float value)
    {
        floats[name] = value;
    }

    void Material::SetInt(const std::string& name, int value)
    {
        ints[name] = value;
    }

    void Material::SetVector2(const std::string& name, Vector2 value)
    {
        vectors2[name] = value;
    }

    void Material::SetVector3(const std::string& name, Vector3 value)
    {
        vectors3[name] = value;
    }

    void Material::SetVector4(const std::string& name, const Vector4& value)
    {
        vectors4[name] = value;
    }

    Texture* Material::GetTexture(const std::string& name)
    {
        const Texture* texture = static_cast<const Material*>(this)->GetTexture(name);
        return const_cast<Texture*>(texture);
    }

    const Texture* Material::GetTexture(const std::string& name) const
    {
        if(auto it = textures.find(name); it != textures.end())
        {
            return it->second;
        }

        return nullptr;
    }
    
    void Material::SetTexture(const std::string& name, const Texture *texture)
    {
        textures[name] = texture;
    }
    
    bool Material::IsTransparent() const
    {
        return transparent;
    }
    
    void Material::SetTransparent(bool aTransparent)
    {
        transparent = aTransparent;
    }
    
    void Material::SetDrawFace(eDrawFace face)
    {
        drawFace = face;
    }
    
    void Material::Apply()
    {
        if(shader)
        {
            shader->StartUse();
            ApplyStates();
        }
    }
    
    void Material::ApplyStates()
    {
        Render::SetDrawFace(drawFace);
    }
}
