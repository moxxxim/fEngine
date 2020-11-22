//#pragma once
//
//#include <FEngine/ScenesManager/GameObjectScript.h>
//
//namespace fengine
//{
//    class Input;
//}
//
//class CameraController final : public fengine::GameObjectScript
//{
//public:
//	CameraController() = default;
//	~CameraController() = default;
//
//	void Start() override;
//	void Update(float deltaTime) override;
//
//	inline void SetCamera(fengine::Transform *cameraTransform);
//	inline void SetInputManager(fengine::Input *inputManager);
//
//	void Serialize(fengine::SerializationNode& node) const override;
//	void Deserialize(const fengine::SerializationNode& node) override;
//
//private:
//
//	void ApplyCamAction(unsigned char key, float delta);
//	void TiltCam(float degree);
//	void PanCam(float degree);
//	void MoveCamX(float distance);
//	void MoveCamZ(float distance);
//
//	fengine::Transform *m_cameraTransform = nullptr;
//	fengine::Input *m_input = nullptr;
//
//	float m_moveSpeed = 1;
//	float m_rotateSpeed = 1;
//	
//	static const unsigned char k_camMoveLeftKey = (unsigned char)65;
//	static const unsigned char k_camMoveRightKey = (unsigned char)68;
//	static const unsigned char k_camMoveForwardKey = (unsigned char)87;
//	static const unsigned char k_camMoveBackwardKey = (unsigned char)83;
//	static const unsigned char k_camTiltUpKey = (unsigned char)38;
//	static const unsigned char k_camTiltDownKey = (unsigned char)40;
//	static const unsigned char k_camPanLeftKey = (unsigned char)37;
//	static const unsigned char k_camPanRightKey = (unsigned char)39;
//};
//
//inline void CameraController::SetCamera(fengine::Transform *cameraTransform)
//{
//	m_cameraTransform = cameraTransform;
//}
//
//inline void CameraController::SetInputManager(fengine::Input *inputManager)
//{
//	m_input = inputManager;
//}
