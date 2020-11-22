//#pragma once
//
//#include <FEngine/ScenesManager/GameObjectScript.h>
//
//class GameObjectRotation final : public fengine::GameObjectScript
//{
//public:
//	GameObjectRotation() = default;
//	~GameObjectRotation() = default;
//
//	void Start() override;
//	void Update(float deltaTime) override;
//
//	void Serialize(fengine::SerializationNode& node) const override;
//	void Deserialize(const fengine::SerializationNode& node) override;
//
//private:
//	float m_fullTurnDuration = 8;
//	float m_oneTurnTimePassed = 0;
//	fengine::Transform *m_transform = nullptr;
//};
