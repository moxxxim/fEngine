//#pragma once
//
//#include <FEngine/ScenesManager/PostEffectPassBehaviour.h>
//
//namespace feng
//{
//    class BlurPassBehaviour final : public PostEffectPassBehaviour
//    {
//    public:
//        BlurPassBehaviour() = default;
//        BlurPassBehaviour(float *blurStrength, int passesCount);
//
//        void OnSerialize(SerializationNode& node) const override;
//        void OnDeserialize(const SerializationNode& node) override;
//
//        void Initialize(PostEffectPrePass *prePass) override;
//        void SetupPass(int passIndex, PostEffectPass *pass) override;
//
//    private:
//        void SetupMaterial(int passIndex, Material *material);
//
//        float *m_blurStrength = nullptr;
//    };
//}
