//#include <FEngine/ScenesManager/BlurPassBehaviour.h>
//
//fengine::BlurPassBehaviour::BlurPassBehaviour(float *blurStrength, int passesCount) :
//	m_blurStrength(blurStrength)
//{
//}
//
//void fengine::BlurPassBehaviour::OnSerialize(SerializationNode& node) const
//{
//	node.SetFloatsArray("BlurStrength", m_blurStrength, GetPassesCount());
//}
//
//void fengine::BlurPassBehaviour::OnDeserialize(const SerializationNode& node)
//{
//	node.TryGetFloatsArray("BlurStrength", m_blurStrength);
//}
//
//void fengine::BlurPassBehaviour::Initialize(PostEffectPrePass *prePass)
//{
//	prePass->SetCustomBuffersCount(2);
//}
//
//void fengine::BlurPassBehaviour::SetupPass(int passIndex, PostEffectPass *pass)
//{
//	pass->SetTextureFromOriginal(PostEffectPass::InputTextureType::Color, "u_colorOriginal");
//	pass->SetTextureFromOriginal(PostEffectPass::InputTextureType::Depth, "u_depthOriginal");
//
//	if (passIndex == GetPassesCount() - 1)
//	{
//		pass->SetRenderBufferOutput();
//	}
//	else
//	{
//		pass->SetRenderBufferCustom(passIndex % 2);
//	}
//
//	if (passIndex == 0)
//	{
//		pass->SetTextureFromInput(fengine::PostEffectPass::InputTextureType::Color, "u_colorProcessed");
//	}
//	else
//	{
//		pass->SetTextureFromCustom(fengine::PostEffectPass::InputTextureType::Color, "u_colorProcessed", (passIndex + 1) % 2);
//	}
//
//	SetupMaterial(passIndex, pass->GetMaterial());
//}
//
//void fengine::BlurPassBehaviour::SetupMaterial(int passIndex, Material *material)
//{
//	float x = 1.0 / Globals::screenWidth;
//	float y = 1.0 / Globals::screenHeight;
//	float z = sqrt(2.0) / 2.0 * x;
//	float w = sqrt(2.0) / 2.0 * y;
//	float pixelSize = m_blurStrength[passIndex];
//	Vector4 step = Vector4(pixelSize * x, pixelSize * y, pixelSize * z, pixelSize * w);
//	material->SetVector4("u_step", step);
//}
