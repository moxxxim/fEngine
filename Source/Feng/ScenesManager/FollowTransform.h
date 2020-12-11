//#pragma once
//
//#include "../Utilities/utilities.h"
//#include "GameObjectScript.h"
//
//namespace feng
//{
//
//class FollowTransform final : public GameObjectScript
//{
//public:
//
//	FollowTransform() = default;
//	~FollowTransform() = default;
//
//	void Start() override;
//	void Update(float deltaTime) override;
//	void Serialize(feng::SerializationNode& node) const override;
//	void Deserialize(const feng::SerializationNode& node) override;
//
//	inline void SetTarget(feng::GameObject *target);
//	inline void SetPositionFollow(bool isFollowPosition);
//	inline void SetRotationFollow(bool isFollowRotation);
//
//private:
//
//	feng::GameObject *m_target = nullptr;
//	bool m_isFollowPosition = false;
//	bool m_isFollowRotation = false;
//	float m_smoothness = 0.5;
//};
//
//inline void FollowTransform::SetTarget(feng::GameObject *target)
//{
//	m_target = target;
//}
//
//inline void FollowTransform::SetPositionFollow(bool isFollowPosition)
//{
//	m_isFollowPosition = isFollowPosition;
//}
//
//inline void FollowTransform::SetRotationFollow(bool isFollowRotation)
//{
//	m_isFollowRotation = isFollowRotation;
//}
//
//}
