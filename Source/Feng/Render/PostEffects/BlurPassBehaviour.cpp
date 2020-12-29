#include <Feng/Render/PostEffects/BlurPassBehaviour.h>

#include <Feng/App/Globals.h>
#include <Feng/Render/PostEffects/PostEffectPass.h>
#include <Feng/Render/PostEffects/PostEffectPrePass.h>
#include <Feng/ResourcesManager/Material.h>
#include <Feng/Utils/Render/ShaderParams.h>

feng::BlurPassBehaviour::BlurPassBehaviour(float *blurStrength, int passesCount) :
	m_blurStrength(blurStrength)
{ }

void feng::BlurPassBehaviour::Initialize(PostEffectPrePass& prePass)
{
    prePass.SetCustomBuffersCount(2);
}

void feng::BlurPassBehaviour::SetupPass(int passIndex, PostEffectPass& pass)
{
    pass.SetTextureFromOriginal(PostEffectPass::InputTextureType::Color, ShaderParams::ScreenColorOriginal.data());
    pass.SetTextureFromOriginal(PostEffectPass::InputTextureType::DepthStencil, ShaderParams::ScreenDepthOriginal.data());

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
                            feng::PostEffectPass::InputTextureType::Color,
                            ShaderParams::ScreenColorProcessed.data());
	}
	else
	{
        pass.SetTextureFromCustom(
                            feng::PostEffectPass::InputTextureType::Color,
                            ShaderParams::ScreenColorProcessed.data(),
                            (passIndex + 1) % 2);
	}

    SetupMaterial(passIndex, pass.GetMaterial());
}

void feng::BlurPassBehaviour::SetupMaterial(int passIndex, Material *material)
{
	float x = 1.0 / screen::ScreenWidth;
	float y = 1.0 / screen::ScreenHeight;
	float z = sqrt(2.0) / 2.0 * x;
	float w = sqrt(2.0) / 2.0 * y;
	float pixelSize = m_blurStrength[passIndex];
	Vector4 step = Vector4(pixelSize * x, pixelSize * y, pixelSize * z, pixelSize * w);
	material->SetVector4("u_step", step);
}
