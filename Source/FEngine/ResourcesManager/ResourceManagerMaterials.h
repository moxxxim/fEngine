//#pragma once
//
//#include "ResourceManager.h"
//#include "Material.h"
//#include "CachedMaterial.h"
//
//namespace feng
//{
//    class ResourceManagerTextures;
//    class ResourceManagerShaders;
//
//    class ResourceManagerMaterials final : public ResourceManager<Material, CachedMaterial>
//    {
//    public:
//        ResourceManagerMaterials(
//                                 CachedMaterial *cachedMaterials,
//                                 unsigned cachedResourcesCount,
//                                 ResourceManagerTextures *texturesManager,
//                                 ResourceManagerShaders *shadersManager);
//
//    protected:
//        Material * Instantiate(CachedMaterial cachedMaterial) override;
//        ResourceManagerTextures *m_materialsManager;
//        ResourceManagerShaders *m_shadersManager;
//    };
//}
