//#pragma once
//
//#include <FEngine/App/Object.h>
//#include <FEngine/Serialization/SerializableObject.h>
//#include <FEngine/Serialization/SerializationNode.h>
//#include <vector>
//
//namespace fengine
//{
//
//class GameObject;
//class Transform;
//
//class GameObjectComponent : public SerializableObject
//{
//	friend GameObject;
//
//public:
//
//	GameObjectComponent();
//	virtual ~GameObjectComponent();
//
//	void Serialize(SerializationNode& node) const override = 0;
//	void Deserialize(const SerializationNode& node) override = 0;
//	void Destroy() override final;
//
//	bool IsGameObjectEnabled() const;
//	inline GameObject * GetGameObject() const;
//	Transform * GetTransform() const;
//
//private:
//
//	inline void SetOwner(GameObject *owner);
//
//	GameObject *m_owner = nullptr;
//
//};
//
//inline fengine::GameObject * fengine::GameObjectComponent::GetGameObject() const
//{
//	return m_owner;
//}
//
//void fengine::GameObjectComponent::SetOwner(GameObject * owner)
//{
//	m_owner = owner;
//}
//
//}
