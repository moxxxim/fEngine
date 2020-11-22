//#pragma once
//
//#include "PostEffect.h"
//#include "../Utils/StringMap.h"
//#include "../Utils/Vector.h"
//
//namespace fengine
//{
//
//class RenderPostProcessing;
//class FrameBuffersPool;
//class PostEffectRenderer;
//class PostEffectPassBehaviour;
//class PostEffectPass;
//
//class MaterialPostEffect : public PostEffect
//{
//
//	friend RenderPostProcessing;
//
//public:
//
//	virtual ~MaterialPostEffect();
//
//	void Apply(PostEffectContext context) override final;
//
//private:
//
//	MaterialPostEffect(Material *material, FrameBuffersPool *buffersPool);
//	MaterialPostEffect(Material *material, FrameBuffersPool *buffersPool, PostEffectPassBehaviour *passBehaviour);
//
//	void Draw();
//	void ApplyCustomBehaviour();
//	void ApplyGenericBehaviour();
//
//	PostEffectRenderer *m_renderer = nullptr;
//	PostEffectPassBehaviour *m_passBehaviour = nullptr;
//	PostEffectPass *m_pass = nullptr;
//	StringMap<unsigned> m_shaderInputTextures;
//
//};
//
//}
