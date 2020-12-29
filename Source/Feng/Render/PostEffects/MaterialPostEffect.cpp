
#include <Feng/Render/PostEffects/MaterialPostEffect.h>

#include <Feng/Render/PostEffects/PostEffectPass.h>
#include <Feng/Render/PostEffects/PostEffectPassBehaviour.h>
#include <Feng/Render/PostEffects/PostEffectPrePass.h>
#include <Feng/Render/PostEffects/PostEffectRenderer.h>
#include <Feng/Utils/Render/ShaderParams.h>

namespace feng
{
    MaterialPostEffect::MaterialPostEffect(feng::Material *aMaterial, FrameBuffersPool *aBuffersPool) :
        MaterialPostEffect(aMaterial, aBuffersPool, nullptr)
    { }

    MaterialPostEffect::MaterialPostEffect(
                                           feng::Material *aMaterial,
                                           FrameBuffersPool *aBuffersPool,
                                           feng::PostEffectPassBehaviour *aPassBehaviour)
        : passBehaviour { aPassBehaviour }
        , renderer { std::make_unique<PostEffectRenderer>(aMaterial, shaderInputTextures)}
    {
        int32_t customBuffersCount = 0;
        if (passBehaviour != nullptr)
        {
            feng::PostEffectPrePass prePass;
            passBehaviour->Initialize(prePass);
            customBuffersCount = prePass.GetCustomBuffersCount();
        }

        pass = std::make_unique<PostEffectPass>(aMaterial, shaderInputTextures, customBuffersCount, aBuffersPool);
    }

    MaterialPostEffect::~MaterialPostEffect() = default;

    void feng::MaterialPostEffect::Apply(const PostEffectContext& context)
    {
        pass->Setup(context);

        if (passBehaviour != nullptr)
        {
            ApplyCustomBehaviour();
        }
        else
        {
            ApplyGenericBehaviour();
        }

        pass->Reset();
    }

    void feng::MaterialPostEffect::ApplyCustomBehaviour()
    {
        int32_t passesCount = passBehaviour->GetPassesCount();
        for (int32_t i = 0; i < passesCount; ++i)
        {
            passBehaviour->SetupPass(i, *pass);
            Draw();
        }
    }

    void MaterialPostEffect::ApplyGenericBehaviour()
    {
        pass->SetTextureFromOriginal(PostEffectPass::InputTextureType::Color, ShaderParams::ScreenColorOriginal.data());
        pass->SetTextureFromOriginal(PostEffectPass::InputTextureType::DepthStencil, ShaderParams::ScreenDepthOriginal.data());
        pass->SetTextureFromInput(PostEffectPass::InputTextureType::Color, ShaderParams::ScreenColorProcessed.data());
        pass->SetRenderBufferOutput();

        Draw();
    }

    void MaterialPostEffect::Draw()
    {
        renderer->Draw();
    }
}
