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
    
    const std::map<std::string, float>& ShaderBindings::GetFloats() const
    {
        return floats;
    }

    bool ShaderBindings::TryGetFloat(const std::string& name, float& value) const
    {
        return TryGetValue<float>(floats, name, value);
    }
    
    void ShaderBindings::SetFloat(const std::string& name, float value)
    {
        floats[name] = value;
    }
    
    const std::map<std::string, int>& ShaderBindings::GetInts() const
    {
        return ints;
    }

    bool ShaderBindings::TryGetInt(const std::string& name, int& value) const
    {
        return TryGetValue<int>(ints, name, value);
    }

    void ShaderBindings::SetInt(const std::string& name, int value)
    {
        ints[name] = value;
    }
    
    const std::map<std::string, Vector2>& ShaderBindings::GetVectors2() const
    {
        return vectors2;
    }

    bool ShaderBindings::TryGetVector2(const std::string& name, Vector2& value) const
    {
        return TryGetValue<Vector2>(vectors2, name, value);
    }

    void ShaderBindings::SetVector2(const std::string& name, Vector2 value)
    {
        vectors2[name] = value;
    }

    bool ShaderBindings::TryGetVector3(const std::string& name, Vector3& value) const
    {
        return TryGetValue<Vector3>(vectors3, name, value);
    }

    const std::map<std::string, Vector4>& ShaderBindings::GetVectors4() const
    {
        return vectors4;
    }
    
    void ShaderBindings::SetVector3(const std::string& name, Vector3 value)
    {
        vectors3[name] = value;
    }
    
    const std::map<std::string, Vector3>& ShaderBindings::GetVectors3() const
    {
        return vectors3;
    }

    bool ShaderBindings::TryGetVector4(const std::string& name, Vector4& value) const
    {
        return TryGetValue<Vector4>(vectors4, name, value);
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

    const std::map<std::string, const Texture*>& ShaderBindings::GetTextures() const
    {
        return textures;
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
    
    bool ShaderBindings::TryGetArrayFloats(const std::string& name, std::vector<float>& values) const
    {
        return TryGetValue<std::vector<float>>(arrayFloats, name, values);
    }
    
    void ShaderBindings::SetArrayFloats(const std::string& name, const std::vector<float>& values)
    {
        arrayFloats[name] = values;
    }
    
    bool ShaderBindings::TryGetArrayMatrices4(const std::string& name, std::vector<Matrix4>& values) const
    {
        return TryGetValue<std::vector<Matrix4>>(arrayMatrices, name, values);
    }
    
    void ShaderBindings::SetArrayMatrices4(const std::string& name, const std::vector<Matrix4>& values)
    {
        arrayMatrices[name] = values;
    }

    bool ShaderBindings::TryGetBuffer(const std::string& name, uint32_t& buffer) const
    {
        return TryGetValue<uint32_t>(buffers, name, buffer);
    }
    
    void ShaderBindings::SetBuffer(const std::string& name, uint32_t buffer)
    {
        buffers[name] = buffer;
    }
    
    template <typename T>
    bool ShaderBindings::TryGetValue(const std::map<std::string, T>& values, const std::string& name, T& value)
    {
        if (auto it = values.find(name); it != values.end())
        {
            value = it->second;
            return true;
        }

        return false;
    }
}
