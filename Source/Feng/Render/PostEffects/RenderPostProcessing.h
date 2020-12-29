#pragma once

#include <Feng/Render/FrameBuffersPool.h>
#include <memory>
#include <vector>

namespace feng
{
    class PostEffect;
    class PostEffectDefinition;
    class FrameBuffer;

    class RenderPostProcessing final
    {
    public:
        RenderPostProcessing();
        ~RenderPostProcessing();
        bool HasPostEffects();
        void SetPostEffect(PostEffectDefinition &postEffect);
        void AddPostEffect(PostEffectDefinition &postEffect);
        void RemoveEffects();
        void ApplyPostEffects(const FrameBuffer& screenBuffer);

    private:
        void ApplyPostEffectsSequence(const FrameBuffer& screenBuffer);
        void CreateQuadBuffer();
        std::unique_ptr<PostEffect> CreatePostEffect(PostEffectDefinition &postEffect);

        std::vector<std::unique_ptr<PostEffect>> effects;
        FrameBuffersPool buffersPool;
        uint32_t vao = 0;
        uint32_t vbo = 0;
    };
}
