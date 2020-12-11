#pragma once

namespace feng
{
    typedef unsigned long int obj_id;
    class Texture;
    class Mesh;
    class Shader;
    class Material;
    class PostEffectDefinition;
    class ResourceManagerTextures;
    class ResourceManagerMeshes;
    class ResourceManagerShaders;
    class ResourceManagerMaterials;
    class ResourceManagerPostEffects;
    class ResourcesSerializer;

    class ResourcesManager final
    {
        friend ResourcesSerializer;

    public:
        unsigned int GetTextureInstancesCount() const;
        unsigned int GetMeshInstancesCount() const;
        unsigned int GetShaderInstancesCount() const;
        Texture * GetTexture(obj_id id);
        Mesh * GetMesh(obj_id id);
        Shader * GetShader(obj_id id);
        Material * GetMaterial(obj_id id);
        PostEffectDefinition * GetPostEffect(obj_id id);

        void Purge();
        bool DestroyResource(unsigned long id);

        static ResourcesManager * GetInstance();

    private:
        ResourcesManager();
        ~ResourcesManager();

        ResourceManagerTextures *m_texturesManager;
        ResourceManagerMeshes *m_meshesManager;
        ResourceManagerShaders *m_shadersManager;
        ResourceManagerMaterials *m_materialsManager;
        ResourceManagerPostEffects *m_postEffectsManager;
    };
}
