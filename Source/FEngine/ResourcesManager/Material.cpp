//#include <FEngine/ResourcesManager/Material.h>
//
//#include <FEngine/ResourcesManager/ResourceManager.h>
//#include <FEngine/Utils/Debug.h>
//
//namespace fengine
//{
//    bool Material::HasTexture(const char *name) const
//    {
//        return m_textures.HasKey(name);
//    }
//
//    bool Material::HasFloat(const char *name) const
//    {
//        return m_floats.HasKey(name);
//    }
//
//    bool Material::HasVector4(const char *name) const
//    {
//        return m_vectors4.HasKey(name);
//    }
//
//    const Texture * Material::GetTexture(const char *name) const
//    {
//        Texture *texture = nullptr;
//        if (m_textures.TryGetValue(name, texture))
//        {
//            return texture;
//        }
//
//        Debug::LogError("Attempt to get material's texture with wrong name.");
//        return nullptr;
//    }
//
//    float Material::GetFloat(const char *name) const
//    {
//        float value;
//        if (m_floats.TryGetValue(name, value))
//        {
//            return value;
//        }
//
//        return 0;
//    }
//
//    Vector4 Material::GetVector4(const char *name) const
//    {
//        Vector4 value;
//        if (m_vectors4.TryGetValue(name, value))
//        {
//            return value;
//        }
//
//        return Vector4(0, 0, 0, 0);
//    }
//
//    void Material::SetTexture(const char *name, Texture *texture)
//    {
//        if (m_textures.HasKey(name))
//        {
//            m_textures[name] = texture;
//        }
//        else
//        {
//            m_textures.Add(name, texture);
//        }
//    }
//
//    void Material::SetFloat(const char *name, float value)
//    {
//        if (m_floats.HasKey(name))
//        {
//            m_floats[name] = value;
//        }
//        else
//        {
//            m_floats.Add(name, value);
//        }
//    }
//
//    void Material::SetVector4(const char *name, const Vector4& value)
//    {
//        if (m_vectors4.HasKey(name))
//        {
//            m_vectors4[name] = value;
//        }
//        else
//        {
//            m_vectors4.Add(name, value);
//        }
//    }
//
//    void Material::Destroy()
//    {
//        ResourcesManager *manager = ResourcesManager::GetInstance();
//        manager->DestroyResource(GetId());
//    }
//}
