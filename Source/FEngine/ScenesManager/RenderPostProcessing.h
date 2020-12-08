//#pragma once
//
//#include "PostEffectContext.h"
//#include "FrameBuffersPool.h"
//#include "../ResourcesManager/PostEffectDefinition.h"
//
//namespace feng
//{
//
//class PostEffect;
//
//class RenderPostProcessing final
//{
//
//public:
//
//	RenderPostProcessing() = default;
//	~RenderPostProcessing();
//	bool HasPostEffects();
//	void SetPostEffect(PostEffectDefinition *postEffect);
//	void AddPostEffect(PostEffectDefinition *postEffect);
//	void RemoveEffects();
//	void ApplyPostEffects(const FrameBuffer& screenBuffer);
//
//	static RenderPostProcessing * GetInstance();
//
//private:
//
//	void ApplyPostEffectsSequence(const FrameBuffer& screenBuffer);
//	void BindQuadBuffer();
//	void UnbindQuadBuffer();
//	void CreateQuadBuffer();
//	PostEffect * CreatePostEffect(PostEffectDefinition *postEffect);
//
//	Vector<PostEffect*> m_effectsSequence;
//	FrameBuffersPool m_buffersPool;
//	unsigned m_vbo = 0;
//	unsigned m_ibo = 0;
//
//};
//
//}
