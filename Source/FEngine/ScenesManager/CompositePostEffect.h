//#pragma once
//
//#include <FEngine/ScenesManager/PostEffect.h>
//
//namespace feng
//{
//    class RenderPostProcessing;
//    class FrameBuffersPool;
//
//    class CompositePostEffect final : public PostEffect
//    {
//        friend RenderPostProcessing;
//
//    public:
//        void Apply(PostEffectContext context) override;
//
//    private:
//        CompositePostEffect(PostEffect **effects, unsigned effectsCount, FrameBuffersPool *buffersPool);
//        ~CompositePostEffect();
//
//        PostEffect **m_effects = nullptr;
//        unsigned m_effectsCount = 0;
//        FrameBuffersPool *m_buffersPool = nullptr;
//    };
//}
