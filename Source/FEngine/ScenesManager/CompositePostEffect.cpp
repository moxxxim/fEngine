//#include <FEngine/ScenesManager/CompositePostEffect.h>
//
//#include <FEngine/ScenesManager/FrameBuffersPool.h>
//
//namespace fengine
//{
//    CompositePostEffect::CompositePostEffect(PostEffect **effects, unsigned effectsCount, FrameBuffersPool *buffersPool) :
//        m_effects(effects),
//        m_effectsCount(effectsCount),
//        m_buffersPool(buffersPool)
//    {
//    }
//
//    CompositePostEffect::~CompositePostEffect()
//    {
//        for (unsigned i = 0; i < m_effectsCount; ++i)
//        {
//            delete m_effects[i];
//        }
//    }
//
//    void CompositePostEffect::Apply(PostEffectContext context)
//    {
//        if (m_effectsCount > 1)
//        {
//            FrameBuffer intermediateBuffer = m_buffersPool->GetBuffer(false);
//
//            for (unsigned i = 0; i < m_effectsCount; ++i)
//            {
//                bool isFirst = i == 0;
//                bool isLast = i == m_effectsCount - 1;
//
//                FrameBuffer input = isFirst ? context.Input : intermediateBuffer;
//                FrameBuffer output = isLast ? context.Output : intermediateBuffer;
//                PostEffectContext context(context.Original, input, output);
//                PostEffect *effect = m_effects[i];
//                effect->Apply(context);
//            }
//        }
//        else if (m_effectsCount == 1)
//        {
//            m_effects[0]->Apply(context);
//        }
//    }
//}
