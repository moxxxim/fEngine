#pragma once

namespace feng
{
    class ResourcesManager;
    class ResourceManagerTextures;
    class ResourceManagerMeshes;
    class ResourceManagerShaders;
    class ResourceManagerMaterials;
    class ResourceManagerPostEffects;
    class CachedTexture;
    class CachedMesh;
    class CachedShader;
    class CachedMaterial;
    class CachedPostEffect;
    class CachedPostEffectDefinition;

    class ResourcesSerializer final
    {
        friend ResourcesManager;

    public:
        void Serialize(const ResourcesManager& resourcesManager);
        int Deserialize(ResourcesManager& resourcesManager);

    private:
        ResourcesSerializer();

        ResourceManagerTextures * ReadTextures(FILE *file);
        ResourceManagerMeshes * ReadMeshes(FILE *file);
        ResourceManagerShaders * ReadShaders(FILE *file);
        ResourceManagerMaterials * ReadMaterials(FILE *file, ResourceManagerTextures *texturesManager, ResourceManagerShaders *shadersManager);
        ResourceManagerPostEffects * ReadPostEffects(FILE *file, ResourceManagerMaterials *materialsManager);
        void ReadTexture(FILE *file, CachedTexture *texture);
        void ReadMesh(FILE *file, CachedMesh *mesh);
        void ReadShader(FILE *file, CachedShader *shader);
        void ReadShaderStates(FILE *file, CachedShader *shader);
        void ReadMaterial(FILE *file, CachedMaterial *material);
        void ReadPostEffect(FILE *file, CachedPostEffectDefinition *postEffect);
        void ReadMaterialTextures(FILE *file, CachedMaterial *material);
        void ReadMaterialFloats(FILE *file, CachedMaterial *material);
        void ReadMaterialVec4(FILE *file, CachedMaterial *material);
	
        void TrimPathQuotes(char *path);
        void ReadVector4(FILE *file, Vector4& vec);

        static const char *k_resources;
        static const char *k_texturesTag;
        static const char *k_meshesTag;
        static const char *k_shadersTag;
        static const char *k_materialsTag;
        static const char *k_postEffectsTag;
        static const char *k_countTag;
        static const char *k_idTag;
        static const char *k_locationTag;
        static const char *k_textureTypeTag;
        static const char *k_textureWrappingTag;
        static const char *k_textureMinFilterTag;
        static const char *k_textureMagFilterTag;
        static const char *k_textureMipMapsTag;
        static const char *k_shaderVsTag;
        static const char *k_shaderFsTag;
        static const char *k_shaderStatesTag;
        static const char *k_shaderAttributesTag;
        static const char *k_shaderUniformsTag;
        static const char *k_materialShaderTag;
        static const char *k_materialTexturesTag;
        static const char *k_materialFloatsTag;
        static const char *k_materialVec4Tag;
        static const char *k_postEffectTypeTag;
        static const char *k_postEffectMaterialTag;
        static const char *k_postEffectPassBehaviourTag;
        static const char *k_postEffectSequenceTag;
    };
}
