#pragma once

#include <Feng/Render/FrameBuffersPool.h>
#include <Feng/Render/VertexBuffer.h>
#include <memory>
#include <vector>

namespace Feng
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
        void ApplyPostEffects(const FrameBuffer& screenBuffer, const FrameBuffer& outBuffer);

    private:
        void ApplyPostEffectsSequence(const FrameBuffer& screenBuffer, const FrameBuffer& outBuffer);
        std::unique_ptr<PostEffect> CreatePostEffect(PostEffectDefinition &postEffect);

        std::vector<std::unique_ptr<PostEffect>> effects;
        FrameBuffersPool buffersPool;
        VertexBuffer quadBuffer;
    };
}
