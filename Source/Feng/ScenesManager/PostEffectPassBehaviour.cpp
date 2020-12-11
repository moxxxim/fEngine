//#include <Feng/ScenesManager/PostEffectPassBehaviour.h>
//
//namespace feng
//{
//    void PostEffectPassBehaviour::Destroy()
//    {
//    }
//
//    void PostEffectPassBehaviour::Serialize(SerializationNode& node) const
//    {
//        node.SetLong("PassesCount", m_passesCount);
//        OnSerialize(node);
//    }
//
//    void PostEffectPassBehaviour::Deserialize(const SerializationNode& node)
//    {
//        long passesCount;
//        node.TryGetLong("PassesCount", passesCount);
//        m_passesCount = passesCount;
//        OnDeserialize(node);
//    }
//}
