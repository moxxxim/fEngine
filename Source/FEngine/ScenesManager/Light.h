//#pragma once
//
//#include "GameObjectComponent.h"
//#include "../../../Utilities/utilities.h"
//
//namespace fengine
//{
//
//class Light final : public GameObjectComponent
//{
//
//public:
//
//	enum class Type : int
//	{
//		Directional = 0,
//		Point = 1
//	};
//
//	Light();
//	~Light() = default;
//
//	void Serialize(SerializationNode& node) const override;
//	void Deserialize(const SerializationNode& node) override;
//
//	inline Type GetType() const;
//	inline Vector4 GetColor() const;
//	inline float GetIntecity() const;
//	inline float GetRange() const;
//
//private:
//
//	Type m_type = Type::Directional;
//	Vector4 m_color;
//	float m_intensity = 1;
//	float m_range = 0;
//
//};
//
//inline Light::Type Light::GetType() const
//{
//	return m_type;
//}
//
//inline Vector4 Light::GetColor() const
//{
//	return m_color;
//}
//
//inline float Light::GetIntecity() const
//{
//	return m_intensity;
//}
//
//inline float Light::GetRange() const
//{
//	return m_range;
//}
//
//}
