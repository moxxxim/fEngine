#include <Feng/Render/ShaderBindings.hpp>

namespace Feng
{
    bool ShaderBindings::HasTexture(const std::string& name) const
    {
        return textures.find(name) != textures.end();
    }

    bool ShaderBindings::HasFloat(const std::string& name) const
    {
        return floats.find(name) != floats.end();
    }

    bool ShaderBindings::HasVector4(const std::string& name) const
    {
        return vectors4.find(name) != vectors4.end();
    }

    void ShaderBindings::SetFloat(const std::string& name, float value)
    {
        floats[name] = value;
    }

    void ShaderBindings::SetInt(const std::string& name, int value)
    {
        ints[name] = value;
    }

    void ShaderBindings::SetVector2(const std::string& name, Vector2 value)
    {
        vectors2[name] = value;
    }

    void ShaderBindings::SetVector3(const std::string& name, Vector3 value)
    {
        vectors3[name] = value;
    }

    void ShaderBindings::SetVector4(const std::string& name, const Vector4& value)
    {
        vectors4[name] = value;
    }

    Texture* ShaderBindings::GetTexture(const std::string& name)
    {
        const Texture* texture = static_cast<const ShaderBindings*>(this)->GetTexture(name);
        return const_cast<Texture*>(texture);
    }

    const Texture* ShaderBindings::GetTexture(const std::string& name) const
    {
        if(auto it = textures.find(name); it != textures.end())
        {
            return it->second;
        }

        return nullptr;
    }
    
    void ShaderBindings::SetTexture(const std::string& name, const Texture *texture)
    {
        textures[name] = texture;
    }
}
