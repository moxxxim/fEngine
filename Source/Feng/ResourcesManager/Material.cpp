#include <Feng/ResourcesManager/Material.h>

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
            shader->Use();
            ApplyStates();
        }
    }
    
    void Material::ApplyStates()
    {
        Render::SetDrawFace(drawFace);
        Render::SetBlending(transparent);
    }
}
