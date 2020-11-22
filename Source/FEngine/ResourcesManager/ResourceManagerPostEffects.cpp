//#include <FEngine/ResourcesManager/ResourceManagerPostEffects.h>
//
//#include <FEngine/App/GuidGenerator.h>
//#include <FEngine/ResourcesManager/ResourceManagerMaterials.h>
//#include <FEngine/ResourcesManager/PostEffectsSerializer.h>
//
//namespace fengine
//{
//    ResourceManagerPostEffects::ResourceManagerPostEffects(CachedPostEffectDefinition *cachedPostEffects,
//        unsigned cachedResourcesCount,
//        ResourceManagerMaterials *materialsManager):
//        ResourceManager<PostEffectDefinition, CachedPostEffectDefinition>(cachedPostEffects, cachedResourcesCount),
//        m_materialsManager(materialsManager)
//    {
//    }
//
//    PostEffectDefinition * ResourceManagerPostEffects::Instantiate(CachedPostEffectDefinition cachedPostEffect)
//    {
//        if (cachedPostEffect.Type == PostEffectType::Material)
//        {
//            return CreateMaterialPostEffect(cachedPostEffect.Id, cachedPostEffect.MaterialId, cachedPostEffect.PassBehaviourId);
//        }
//        else
//        {
//            return CreateCompositePostEffect(cachedPostEffect.Id, cachedPostEffect.Sequence);
//        }
//    }
//
//    PostEffectDefinition * ResourceManagerPostEffects::CreateCompositePostEffect(obj_id id, std::vector<obj_id> innerSequence)
//    {
//        unsigned sequenceLength = innerSequence.GetSize();
//        Vector<PostEffectDefinition*> sequence(sequenceLength);
//        for (unsigned i = 0; i < sequenceLength; ++i)
//        {
//            PostEffectDefinition *instance = GetInstance(innerSequence[i]);
//            sequence.Add(instance);
//        }
//
//        GuidGenerator::SetNextResourceId(id);
//        return new PostEffectDefinition(sequence);
//    }
//
//    PostEffectDefinition * ResourceManagerPostEffects::CreateMaterialPostEffect(obj_id id, obj_id materialId, obj_id passBehaviourId)
//    {
//        Material *material = m_materialsManager->GetInstance(materialId);
//        PostEffectPassBehaviour *passBehaviour = CreatePassBehaviour(passBehaviourId);
//
//        GuidGenerator::SetNextResourceId(id);
//        return new PostEffectDefinition(material, passBehaviour);
//    }
//
//    PostEffectPassBehaviour * ResourceManagerPostEffects::CreatePassBehaviour(obj_id passBehaviourId)
//    {
//        if (passBehaviourId)
//        {
//            PostEffectSerializer serializer;
//            return serializer.GetPostEffectPassBehaviour(passBehaviourId);
//        }
//
//        return nullptr;
//    }
//}
