//
//#include "PostEffectPassBehaviour.h"
//
//#include "../../Globals.h"
//#include "MaterialPostEffect.h"
//#include "../ResourcesManager/Material.h"
//#include "PostEffectRenderer.h"
//#include "../ResourcesManager/ShaderParameters.h"
//
//fengine::MaterialPostEffect::MaterialPostEffect(fengine::Material *material, FrameBuffersPool *buffersPool) :
//	MaterialPostEffect(material, buffersPool, nullptr)
//{
//}
//
//fengine::MaterialPostEffect::MaterialPostEffect(fengine::Material *material, FrameBuffersPool *buffersPool, fengine::PostEffectPassBehaviour *passBehaviour) :
//	m_renderer(new PostEffectRenderer(material, &m_shaderInputTextures)),
//	m_shaderInputTextures(0),
//	m_passBehaviour(passBehaviour)
//{
//	int customBuffersCount = 0;
//	if (passBehaviour)
//	{
//		fengine::PostEffectPrePass prePass(buffersPool);
//		passBehaviour->Initialize(&prePass);
//		customBuffersCount = prePass.GetCustomBuffersCount();
//	}
//
//	m_pass = new PostEffectPass(material, m_shaderInputTextures, customBuffersCount, buffersPool);
//}
//
//fengine::MaterialPostEffect::~MaterialPostEffect()
//{
//	delete m_renderer;
//	delete m_pass;
//}
//
//void fengine::MaterialPostEffect::Apply(PostEffectContext context)
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
//void fengine::MaterialPostEffect::ApplyCustomBehaviour()
//{
//	int passesCount = m_passBehaviour->GetPassesCount();
//	for (int i = 0; i < passesCount; ++i)
//	{
//		m_passBehaviour->SetupPass(i, m_pass);
//		Draw();
//	}
//}
//
//void fengine::MaterialPostEffect::ApplyGenericBehaviour()
//{
//	const char *originalScreenColorUniform = fengine::ShaderParameters::GetReservedUniformName(fengine::ShaderParameters::ReservedUniform::ScreenColorOriginal);
//	const char *originalScreenDepthUniform = fengine::ShaderParameters::GetReservedUniformName(fengine::ShaderParameters::ReservedUniform::ScreenDepthOriginal);
//	const char *processedScreenColorUniform = fengine::ShaderParameters::GetReservedUniformName(fengine::ShaderParameters::ReservedUniform::ScreenColorProcessed);
//
//	m_pass->SetTextureFromOriginal(fengine::PostEffectPass::InputTextureType::Color, originalScreenColorUniform);
//	m_pass->SetTextureFromOriginal(fengine::PostEffectPass::InputTextureType::Depth, originalScreenDepthUniform);
//	m_pass->SetTextureFromInput(fengine::PostEffectPass::InputTextureType::Color, processedScreenColorUniform);
//	m_pass->SetRenderBufferOutput();
//
//	Draw();
//}
//
//void fengine::MaterialPostEffect::Draw()
//{
//	m_renderer->Draw();
//}
