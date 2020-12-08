//#include <FEngine/ResourcesManager/CachedPostEffectDefinition.h>
//#include <utility>
//
//namespace feng
//{
//    CachedPostEffectDefinition::CachedPostEffectDefinition(const CachedPostEffectDefinition& other):
//        CachedResource(other),
//        Type(other.Type),
//        MaterialId(other.MaterialId),
//        PassBehaviourId(other.PassBehaviourId),
//        Sequence(other.Sequence)
//    {
//    }
//
//    CachedPostEffectDefinition::CachedPostEffectDefinition(CachedPostEffectDefinition&& other) :
//        CachedResource(std::move(other)),
//        Type(other.Type),
//        MaterialId(other.MaterialId),
//        PassBehaviourId(other.PassBehaviourId),
//        Sequence(std::move(other.Sequence))
//    {
//    }
//
//    CachedPostEffectDefinition& CachedPostEffectDefinition::operator=(const CachedPostEffectDefinition& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(other);
//            Type = other.Type;
//            MaterialId = other.MaterialId;
//            PassBehaviourId = other.PassBehaviourId;
//            Sequence = other.Sequence;
//        }
//
//        return *this;
//    }
//
//    CachedPostEffectDefinition& CachedPostEffectDefinition::operator=(CachedPostEffectDefinition&& other)
//    {
//        if (this != &other)
//        {
//            CachedResource::operator=(std::move(other));
//            Type = other.Type;
//            MaterialId = other.MaterialId;
//            PassBehaviourId = other.PassBehaviourId;
//            Sequence = std::move(other.Sequence);
//        }
//
//        return *this;
//    }
//}
