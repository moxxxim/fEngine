//#include <Classes/Scripts/GameObjectRotation.h>
//
//#include <FEngine/ScenesManager/Transform.h>
//
//void GameObjectRotation::Start()
//{
//	m_transform = GetTransform();
//}
//
//void GameObjectRotation::Update(float deltaTime)
//{
////	if (m_transform)
////	{
////		m_oneTurnTimePassed += deltaTime;
////		if (m_oneTurnTimePassed > m_fullTurnDuration)
////		{
////			m_oneTurnTimePassed -= m_fullTurnDuration;
////		}
////
////		m_transform->SetRotation(0, 2 * 3.14 * m_oneTurnTimePassed / m_fullTurnDuration, 0);
////	}
//}
//
//void GameObjectRotation::Serialize(fengine::SerializationNode& node) const
//{
//	node.SetFloat("FullTurnDuration", m_fullTurnDuration);
//}
//
//void GameObjectRotation::Deserialize(const fengine::SerializationNode& node)
//{
//	node.TryGetFloat("FullTurnDuration", m_fullTurnDuration);
//}