//
//#include "PostEffectPassBehaviour.h"
//
//#include "../../Globals.h"
//#include "MaterialPostEffect.h"
//#include "../ResourcesManager/Material.h"
//#include "PostEffectRenderer.h"
//#include "../ResourcesManager/ShaderParameters.h"
//
//feng::MaterialPostEffect::MaterialPostEffect(feng::Material *material, FrameBuffersPool *buffersPool) :
//	MaterialPostEffect(material, buffersPool, nullptr)
//{
//}
//
//feng::MaterialPostEffect::MaterialPostEffect(feng::Material *material, FrameBuffersPool *buffersPool, feng::PostEffectPassBehaviour *passBehaviour) :
//	m_renderer(new PostEffectRenderer(material, &m_shaderInputTextures)),
//	m_shaderInputTextures(0),
//	m_passBehaviour(passBehaviour)
//{
//	int customBuffersCount = 0;
//	if (passBehaviour)
//	{
//		feng::PostEffectPrePass prePass(buffersPool);
//		passBehaviour->Initialize(&prePass);
//		customBuffersCount = prePass.GetCustomBuffersCount();
//	}
//
//	m_pass = new PostEffectPass(material, m_shaderInputTextures, customBuffersCount, buffersPool);
//}
//
//feng::MaterialPostEffect::~MaterialPostEffect()
//{
//	delete m_renderer;
//	delete m_pass;
//}
//
//void feng::MaterialPostEffect::Apply(PostEffectContext context)
//{
//	m_pass->Setup(context);
//
//	if (m_passBehaviour)
//	{
//		ApplyCustomBehaviour();
//	}
//	else
//	{
//		ApplyGenericBehaviour();
//	}
//
//	m_pass->Reset();
//}
//
//void feng::MaterialPostEffect::ApplyCustomBehaviour()
//{
//	int passesCount = m_passBehaviour->GetPassesCount();
//	for (int i = 0; i < passesCount; ++i)
//	{
//		m_passBehaviour->SetupPass(i, m_pass);
//		Draw();
//	}
//}
//
//void feng::MaterialPostEffect::ApplyGenericBehaviour()
//{
//	const char *originalScreenColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::ScreenColorOriginal);
//	const char *originalScreenDepthUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::ScreenDepthOriginal);
//	const char *processedScreenColorUniform = feng::ShaderParameters::GetReservedUniformName(feng::ShaderParameters::ReservedUniform::ScreenColorProcessed);
//
//	m_pass->SetTextureFromOriginal(feng::PostEffectPass::InputTextureType::Color, originalScreenColorUniform);
//	m_pass->SetTextureFromOriginal(feng::PostEffectPass::InputTextureType::Depth, originalScreenDepthUniform);
//	m_pass->SetTextureFromInput(feng::PostEffectPass::InputTextureType::Color, processedScreenColorUniform);
//	m_pass->SetRenderBufferOutput();
//
//	Draw();
//}
//
//void feng::MaterialPostEffect::Draw()
//{
//	m_renderer->Draw();
//}
