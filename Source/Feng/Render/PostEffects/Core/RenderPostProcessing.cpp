#include <Feng/Render/PostEffects/Core/RenderPostProcessing.h>

#include <Feng/Render/PostEffects/Core/CompositePostEffect.h>
#include <Feng/Render/PostEffects/Core/MaterialPostEffect.h>
#include <Feng/Render/PostEffects/Core/PostEffect.h>
#include <Feng/Render/PostEffects/Core/PostEffectContext.h>
#include <Feng/Render/PostEffects/Core/PostEffectDefinition.h>
#include <Feng/Render/RenderBase.h>
#include <Feng/Utils/Debug.h>
#include <Feng/Utils/Render/MeshParams.h>
#include <Feng/Utils/Render/RenderUtils.h>

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

#include <array>

namespace Feng
{
    RenderPostProcessing::RenderPostProcessing() = default;

    RenderPostProcessing::~RenderPostProcessing()
    {
        glDeleteBuffers(1, &vao);
        glDeleteBuffers(1, &vbo);
    }

    bool RenderPostProcessing::HasPostEffects()
    {
        return !effects.empty();
    }

    void RenderPostProcessing::SetPostEffect(PostEffectDefinition &postEffect)
    {
        RemoveEffects();
        AddPostEffect(postEffect);
    }

    void RenderPostProcessing::AddPostEffect(PostEffectDefinition &postEffect)
    {
        std::unique_ptr<PostEffect> effect = CreatePostEffect(postEffect);
        effects.push_back(std::move(effect));
    }

    void RenderPostProcessing::RemoveEffects()
    {
        effects.clear();
    }

    void RenderPostProcessing::ApplyPostEffects(const FrameBuffer& screenBuffer)
    {
        if (vao == 0)
        {
            CreateQuadBuffer();
        }

        glBindVertexArray(vao);
        ApplyPostEffectsSequence(screenBuffer);
        glBindVertexArray(Render::UndefinedBuffer);
    }

    void RenderPostProcessing::ApplyPostEffectsSequence(const FrameBuffer& screenBuffer)
    {
        FrameBuffer::Settings intermediateSettings = screenBuffer.settings;
        intermediateSettings.color = FrameBuffer::eAttachementState::Texture;
        intermediateSettings.depth = FrameBuffer::eAttachementState::None;
        intermediateSettings.stencil = FrameBuffer::eAttachementState::None;
        intermediateSettings.multisample = false;
        intermediateSettings.combinedDepthStencil = false;
        
        size_t effectsCount = effects.size();
        FrameBuffer intermediateBuffer = (effectsCount > 1) ? buffersPool.Pop(intermediateSettings) : FrameBuffer{};

        PostEffectContext context;
        context.original = screenBuffer;

        for (size_t i = 0; i < effectsCount; ++i)
        {
            // TODO: m.alekseev Seems like it doesn't work correctly for multiple effects.
            context.input = (i == 0) ? screenBuffer : intermediateBuffer;
            context.output = (i == (effectsCount - 1)) ? FrameBuffer{} : intermediateBuffer;

            std::unique_ptr<PostEffect> &effect = effects[i];
            effect->Apply(context);
            Print_Errors_OpengGL();
        }

        if (effectsCount > 1)
        {
            buffersPool.Push(intermediateBuffer);
        }
    }

    void RenderPostProcessing::CreateQuadBuffer()
    {
        std::array<float, 6 * (3 + 2)> quadVertices
        {
            -1.0, -1.0, 0.0,    0.0, 0.0,
            -1.0,  1.0, 0.0,    0.0, 1.0,
             1.0, -1.0, 0.0,    1.0, 0.0,

             1.0, -1.0, 0.0,    1.0, 0.0,
            -1.0,  1.0, 0.0,    0.0, 1.0,
             1.0,  1.0, 0.0,    1.0, 1.0
        };

        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quadVertices.size(), quadVertices.data(), GL_STATIC_DRAW);

        uint32_t attributes = eVertexAtributes::Position | eVertexAtributes::Uv0;
        std::ignore = Render::EnableVertexAttributes(static_cast<eVertexAtributes>(attributes));

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(Render::UndefinedBuffer);
    }

    std::unique_ptr<PostEffect> RenderPostProcessing::CreatePostEffect(PostEffectDefinition &effectDefinition)
    {
        PostEffectType type = effectDefinition.GetType();
        if (type == PostEffectType::Material)
        {
            Material* material = effectDefinition.GetMaterial();
            PostEffectPassBehaviour* passBehaviour = effectDefinition.GetPassBehaviour();
            return std::make_unique<MaterialPostEffect>(material, &buffersPool, passBehaviour);
        }
        else
        {
            const std::vector<std::unique_ptr<PostEffectDefinition>>& subsequence = effectDefinition.GetSubsequence();
            int effectsCount = subsequence.size();
            std::vector<std::unique_ptr<PostEffect>> subEffects;
            subEffects.reserve(effectsCount);

            for (uint32_t i = 0; i < effectsCount; ++i)
            {
                subEffects.push_back(CreatePostEffect(*subsequence[i]));
            }

            return std::make_unique<CompositePostEffect>(std::move(subEffects), &buffersPool);
        }
    }
}
