//#pragma once
//
//#include <FEngine/ScenesManager/PostEffectPass.h>
//#include <FEngine/ScenesManager/PostEffectPrePass.h>
//#include <FEngine/Serialization/SerializableObject.h>
//
//namespace fengine
//{
//    class PostEffectPassBehaviour : public SerializableObject
//    {
//    public:
//        PostEffectPassBehaviour();
//
//        void Destroy() override;
//
//        void Serialize(SerializationNode& node) const override final;
//        void Deserialize(const SerializationNode& node) override final;
//
//        inline int GetPassesCount() const;
//        virtual void Initialize(PostEffectPrePass *prePass) = 0;
//        virtual void SetupPass(int passIndex, PostEffectPass *pass) = 0;
//	
//        virtual void OnSerialize(SerializationNode& node) const {};
//        virtual void OnDeserialize(const SerializationNode& node) {};
//
//    private:
//        int m_passesCount = 1;
//    };
//
//    inline PostEffectPassBehaviour::PostEffectPassBehaviour() :
//        SerializableObject(ObjectType::Resource)
//    { }
//
//    inline int PostEffectPassBehaviour::GetPassesCount() const
//    {
//        return m_passesCount;
//    }
//}
