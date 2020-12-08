//#include <Classes/Scripts/PostEffectsSwitcher.h>
//
//void PostEffectsSwitcher::Start()
//{
//	m_input = feng::Input::GetInstance();
//	m_postProcessing = feng::RenderPostProcessing::GetInstance();
//}
//
//void PostEffectsSwitcher::Update(float deltaTime)
//{
//	if (m_input)
//	{
//		bool isKeyPressed = m_input->IsKeyPressed();
//
//		if (isKeyPressed && !wasKeyPressed)
//		{
//			unsigned char key = m_input->GetPressedKey();
//			if (IsKnownKey(key))
//			{
//				EnablePostEffect(key);
//			}
//		}
//
//		wasKeyPressed = isKeyPressed;
//	}
//}
//
//void PostEffectsSwitcher::Serialize(feng::SerializationNode& node) const
//{
//	node.SetLong("BlackNWhiteId", m_blackAndWhite->GetId());
//	node.SetLong("BlurId", m_blur->GetId());
//	node.SetLong("BloomId", m_bloom->GetId());
//	node.SetLong("DepthOfFieldId", m_depthOfField->GetId());
//}
//
//void PostEffectsSwitcher::Deserialize(const feng::SerializationNode& node)
//{
//	long blackNWhiteId;
//	long blurId;
//	long bloomId;
//	long depthOfFieldId;
//
//	node.TryGetLong("BlackNWhiteId", blackNWhiteId);
//	node.TryGetLong("BlurId", blurId);
//	node.TryGetLong("BloomId", bloomId);
//	node.TryGetLong("DepthOfFieldId", depthOfFieldId);
//
//	m_blackAndWhite = GetPostEffect(blackNWhiteId);
//	m_blur = GetPostEffect(blurId);
//	m_bloom = GetPostEffect(bloomId);
//	m_depthOfField = GetPostEffect(depthOfFieldId);
//}
//
//bool PostEffectsSwitcher::IsKnownKey(unsigned char key)
//{
//	return key == k_disableEffectKey ||
//		key == k_enableBlackAndWhiteKey ||
//		key == k_enableBlurKey ||
//		key == k_enableBloomKey ||
//		key == k_enableDepthOfFieldKey;
//}
//
//feng::PostEffectDefinition * PostEffectsSwitcher::GetPostEffect(feng::obj_id id)
//{
//	feng::ResourcesManager *resources = feng::ResourcesManager::GetInstance();
//	return resources->GetPostEffect(id);
//}
//
//void PostEffectsSwitcher::EnablePostEffect(unsigned char key)
//{
//	switch (key)
//	{
//		case k_disableEffectKey:
//		{
//			m_postProcessing->RemoveEffects();
//		}
//		break;
//
//		case k_enableBlackAndWhiteKey:
//		{
//			m_postProcessing->SetPostEffect(m_blackAndWhite);
//		}
//		break;
//
//		case k_enableBlurKey:
//		{
//			m_postProcessing->SetPostEffect(m_blur);
//		}
//		break;
//
//		case k_enableBloomKey:
//		{
//			m_postProcessing->SetPostEffect(m_bloom);
//		}
//		break;
//
//		case k_enableDepthOfFieldKey:
//		{
//			m_postProcessing->SetPostEffect(m_depthOfField);
//		}
//		break;
//
//		default:
//		{
//			m_postProcessing->RemoveEffects();
//		}
//		break;
//	}
//}
