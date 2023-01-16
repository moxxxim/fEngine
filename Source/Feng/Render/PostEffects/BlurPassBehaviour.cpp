#include <Feng/Render/PostEffects/BlurPassBehaviour.h>

#include <Feng/App/Globals.h>
#include <Feng/Render/PostEffects/Core/PostEffectPass.h>
#include <Feng/Render/PostEffects/Core/PostEffectPrePass.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/Utils/Render/ShaderParams.h>

namespace Feng
{
    BlurPassBehaviour::BlurPassBehaviour(float *blurStrength, int passesCount) :
        m_blurStrength(blurStrength)
    { }

    void BlurPassBehaviour::Initialize(PostEffectPrePass& prePass)
    {
        prePass.SetCustomBuffersCount(2);
    }

    void BlurPassBehaviour::SetupPass(int passIndex, PostEffectPass& pass)
    {
        pass.SetTextureFromOriginal(PostEffectPass::InputTextureType::Color, ShaderParams::ScreenColorOriginal.data());
        pass.SetTextureFromOriginal(PostEffectPass::InputTextureType::Depth, ShaderParams::ScreenDepthOriginal.data());

        if (passIndex == GetPassesCount() - 1)
        {
            pass.SetRenderBufferOutput();
        }
        else
        {
            pass.SetRenderBufferCustom(passIndex % 2);
        }

        if (passIndex == 0)
        {
            pass.SetTextureFromInput(
                                PostEffectPass::InputTextureType::Color,
                                ShaderParams::ScreenColorProcessed.data());
        }
        else
        {
            pass.SetTextureFromCustom(
                                PostEffectPass::InputTextureType::Color,
                                ShaderParams::ScreenColorProcessed.data(),
                                (passIndex + 1) % 2);
        }

        SetupMaterial(passIndex, pass.GetMaterial());
    }

    void BlurPassBehaviour::SetupMaterial(int passIndex, Material *material)
    {
        float x = 1.0 / Screen::ScreenWidth;
        float y = 1.0 / Screen::ScreenHeight;
        float z = sqrt(2.0) / 2.0 * x;
        float w = sqrt(2.0) / 2.0 * y;
        float pixelSize = m_blurStrength[passIndex];
        Vector4 step = Vector4(pixelSize * x, pixelSize * y, pixelSize * z, pixelSize * w);
        material->SetVector4("u_step", step);
    }
}
