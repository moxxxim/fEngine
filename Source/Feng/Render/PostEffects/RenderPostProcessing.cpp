#include "RenderPostProcessing.h"
#include "MaterialPostEffect.h"
#include "CompositePostEffect.h"

#include <OpenGL/gl.h>
#include <OpenGL/gl3.h>

namespace feng
{
    RenderPostProcessing::~RenderPostProcessing()
    {
        glDeleteBuffers(1, &vbo);
        glDeleteBuffers(1, &ibo);
    }

    bool RenderPostProcessing::HasPostEffects()
    {
        return !effectsSequence.empty();
    }

    void RenderPostProcessing::SetPostEffect(PostEffectDefinition *postEffect)
    {
        RemoveEffects();
        AddPostEffect(postEffect);
    }

    void RenderPostProcessing::AddPostEffect(PostEffectDefinition *postEffect)
    {
        PostEffect *effect = CreatePostEffect(postEffect);
        effectsSequence.push_back(effect);
    }

    void RenderPostProcessing::RemoveEffects()
    {
        effectsSequence.clear();
    }

    void RenderPostProcessing::ApplyPostEffects(const FrameBuffer& screenBuffer)
    {
        BindQuadBuffer();
        ApplyPostEffectsSequence(screenBuffer);
        UnbindQuadBuffer();
    }

    void RenderPostProcessing::ApplyPostEffectsSequence(const FrameBuffer& screenBuffer)
    {
//        unsigned effectsCount = m_effectsSequence.GetSize();
//        FrameBuffer intermediateBuffer = effectsCount > 1 ? m_buffersPool.GetBuffer(false) : FrameBuffer(0, 0, 0);
//
//        for (unsigned i = 0; i < effectsCount; ++i)
//        {
//            bool isFirst = i == 0;
//            bool isLast = i == effectsCount - 1;
//
//            FrameBuffer input = isFirst ? screenBuffer : intermediateBuffer;
//            FrameBuffer output = isLast ? FrameBuffer(0, 0, 0) : intermediateBuffer;
//            PostEffectContext context(screenBuffer, input, output);
//            PostEffect *effect = m_effectsSequence[i];
//            effect->Apply(context);
//        }
//
//        if (effectsCount > 1)
//        {
//            m_buffersPool.PutBuffer(intermediateBuffer);
//        }
    }

    void RenderPostProcessing::BindQuadBuffer()
    {
//        if (!m_vbo)
//        {
//            CreateQuadBuffer();
//        }
//
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
    }

    void RenderPostProcessing::UnbindQuadBuffer()
    {
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    void RenderPostProcessing::CreateQuadBuffer()
    {
//        Vertex vertices[4];
//        vertices[0].Position = Vector3(-1.0, -1.0, 0.0);
//        vertices[0].UV1 = Vector2(0.0, 0.0);
//
//        vertices[1].Position = Vector3(-1.0, 1.0, 0.0);
//        vertices[1].UV1 = Vector2(0.0, 1.0);
//
//        vertices[2].Position = Vector3(1.0, 1.0, 0.0);
//        vertices[2].UV1 = Vector2(1.0, 1.0);
//
//        vertices[3].Position = Vector3(1.0, -1.0, 0.0);
//        vertices[3].UV1 = Vector2(1.0, 0.0);
//
//        GLuint indices[6] = {0, 1, 2, 0, 2, 3};
//
//        glGenBuffers(1, &m_vbo);
//        glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
//        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
//        glBindBuffer(GL_ARRAY_BUFFER, 0);
//
//        glGenBuffers(1, &m_ibo);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ibo);
//        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
//        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    PostEffect* RenderPostProcessing::CreatePostEffect(PostEffectDefinition *postEffect)
    {
//        PostEffectType type = postEffect->GetType();
//        if (type == PostEffectType::Material)
//        {
//            return new MaterialPostEffect(postEffect->GetMaterial(), &m_buffersPool, postEffect->GetPassBehaviour());
//        }
//        else
//        {
//            const Vector<PostEffectDefinition*>& postEffects = postEffect->GetInnerSequence();
//            int effectsCount = postEffects.GetSize();
//            PostEffect **effects = new PostEffect*[effectsCount];
//
//            for (int i = 0; i < effectsCount; ++i)
//            {
//                effects[i] = CreatePostEffect(postEffects[i]);
//            }
//
//            return new CompositePostEffect(effects, effectsCount, &m_buffersPool);
//        }
    }
}
