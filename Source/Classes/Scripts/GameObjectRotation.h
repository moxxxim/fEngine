//#pragma once
//
//#include <Feng/ScenesManager/GameObjectScript.h>
//
//class GameObjectRotation final : public feng::GameObjectScript
//{
//public:
//	GameObjectRotation() = default;
//	~GameObjectRotation() = default;
//
//	void Start() override;
//	void Update(float deltaTime) override;
//
//	void Serialize(feng::SerializationNode& node) const override;
//	void Deserialize(const feng::SerializationNode& node) override;
//
//private:
//	float m_fullTurnDuration = 8;
//	float m_oneTurnTimePassed = 0;
//	feng::Transform *m_transform = nullptr;
//};
