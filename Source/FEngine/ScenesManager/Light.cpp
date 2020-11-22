//#include "Light.h"
//
//fengine::Light::Light() :
//	m_color(Vector4(0, 0, 0, 0))
//{
//
//}
//
//void fengine::Light::Serialize(SerializationNode& node) const
//{
//	node.SetName("Light");
//	node.SetLong("Type", (int)m_type);
//	node.SetFloat("ColorR", m_color.x);
//	node.SetFloat("ColorG", m_color.y);
//	node.SetFloat("ColorB", m_color.z);
//	node.SetFloat("ColorA", m_color.w);
//	node.SetFloat("Intensity", m_intensity);
//	node.SetFloat("Range", m_range);
//}
//
//void fengine::Light::Deserialize(const SerializationNode& node)
//{
//	long type;
//	node.TryGetLong("Type", type);
//	m_type = (Light::Type)type;
//	node.TryGetFloat("ColorR", m_color.x);
//	node.TryGetFloat("ColorG", m_color.y);
//	node.TryGetFloat("ColorB", m_color.z);
//	node.TryGetFloat("ColorA", m_color.w);
//	node.TryGetFloat("Intensity", m_intensity);
//	node.TryGetFloat("Range", m_range);
//}
