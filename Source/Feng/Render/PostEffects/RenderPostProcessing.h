#pragma once

#include <Feng/Render/FrameBuffersPool.h>
#include <vector>

namespace feng
{
    class PostEffect;
    class PostEffectDefinition;
    class FrameBuffer;

    class RenderPostProcessing final
    {
    public:
        RenderPostProcessing() = default;
        ~RenderPostProcessing();
        bool HasPostEffects();
        void SetPostEffect(PostEffectDefinition *postEffect);
        void AddPostEffect(PostEffectDefinition *postEffect);
        void RemoveEffects();
        void ApplyPostEffects(const FrameBuffer& screenBuffer);

    private:
        void ApplyPostEffectsSequence(const FrameBuffer& screenBuffer);
        void BindQuadBuffer();
        void UnbindQuadBuffer();
        void CreateQuadBuffer();
        PostEffect* CreatePostEffect(PostEffectDefinition *postEffect);

        std::vector<PostEffect*> effectsSequence;
        FrameBuffersPool buffersPool;
        uint32_t vbo = 0;
        uint32_t ibo = 0;
    };
}
