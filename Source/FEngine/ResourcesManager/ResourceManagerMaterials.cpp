//#include <FEngine/ResourcesManager/ResourceManagerMaterials.h>
//
//#include <FEngine/App/GuidGenerator.h>
//#include <FEngine/ResourcesManager/Material.h>
//#include <FEngine/ResourcesManager/ResourceManagerTextures.h>
//#include <FEngine/ResourcesManager/ResourceManagerShaders.h>
//
//namespace feng
//{
//    ResourceManagerMaterials::ResourceManagerMaterials(CachedMaterial *cachedMaterials,
//        unsigned cachedMaterialsCount,
//        ResourceManagerTextures *texturesManager,
//        ResourceManagerShaders *shadersManager) :
//        ResourceManager<Material, CachedMaterial>(cachedMaterials, cachedMaterialsCount),
//        m_materialsManager(texturesManager),
//        m_shadersManager(shadersManager)
//    { }
//
//    Material * ResourceManagerMaterials::Instantiate(CachedMaterial cachedMaterial)
//    {
//        Shader *shader = m_shadersManager->GetInstance(cachedMaterial.ShaderId);
//        Material *material = new Material(shader);
//        auto texturesIterator = cachedMaterial.Textures.GetIterator();
//        while (texturesIterator.HasCurrent())
//        {
//            feng::KeyValuePair<const char *, obj_id> namedTexture = texturesIterator.GetCurrent();
//            Texture *texture = m_materialsManager->GetInstance(namedTexture.Value);
//            material->SetTexture(namedTexture.Key, texture);
//            texturesIterator.MoveNext();
//        }
//
//        auto floatsIterator = cachedMaterial.Floats.GetIterator();
//        while (floatsIterator.HasCurrent())
//        {
//            feng::KeyValuePair<const char *, float> namedFloat = floatsIterator.GetCurrent();
//            material->SetFloat(namedFloat.Key, namedFloat.Value);
//            floatsIterator.MoveNext();
//        }
//
//        auto vec4ValuesIterator = cachedMaterial.Vec4Values.GetIterator();
//        while (vec4ValuesIterator.HasCurrent())
//        {
//            feng::KeyValuePair<const char *, Vector4> namedVector = vec4ValuesIterator.GetCurrent();
//            material->SetVector4(namedVector.Key, namedVector.Value);
//            vec4ValuesIterator.MoveNext();
//        }
//
//        GuidGenerator::SetNextResourceId(cachedMaterial.Id);
//        return material;
//    }
//}
