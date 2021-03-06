//#pragma once
//
//#include <Feng/ResourcesManager/ResourceManager.h>
//
//#include <Feng/ResourcesManager/CachedPostEffectDefinition.h>
//#include <Feng/ResourcesManager/PostEffectDefinition.h>
//
//#include <vector>
//
//namespace feng
//{
//    class ResourceManagerMaterials;
//    class ResourceManagerShaders;
//
//    class ResourceManagerPostEffects final : public ResourceManager<PostEffectDefinition, CachedPostEffectDefinition>
//    {
//    public:
//        ResourceManagerPostEffects(
//                                   CachedPostEffectDefinition *cachedPostEffects,
//                                   unsigned cachedResourcesCount,
//                                   ResourceManagerMaterials *materialsManager);
//
//    protected:
//        PostEffectDefinition * Instantiate(CachedPostEffectDefinition cachedPostEffect) override;
//        ResourceManagerMaterials *m_materialsManager;
//
//    private:
//        PostEffectDefinition * CreateCompositePostEffect(obj_id id, std::vector<obj_id> innerSequence);
//        PostEffectDefinition * CreateMaterialPostEffect(obj_id id, obj_id materialId, obj_id passBehaviourId);
//        PostEffectPassBehaviour * CreatePassBehaviour(obj_id passBehaviourId);
//    };
//}
