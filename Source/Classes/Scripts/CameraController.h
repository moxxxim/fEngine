//#pragma once
//
//#include <FEngine/ScenesManager/GameObjectScript.h>
//
//namespace feng
//{
//    class Input;
//}
//
//class CameraController final : public feng::GameObjectScript
//{
//public:
//	CameraController() = default;
//	~CameraController() = default;
//
//	void Start() override;
//	void Update(float deltaTime) override;
//
//	inline void SetCamera(feng::Transform *cameraTransform);
//	inline void SetInputManager(feng::Input *inputManager);
//
//	void Serialize(feng::SerializationNode& node) const override;
//	void Deserialize(const feng::SerializationNode& node) override;
//
//private:
//
//	void ApplyCamAction(unsigned char key, float delta);
//	void TiltCam(float degree);
//	void PanCam(float degree);
//	void MoveCamX(float distance);
//	void MoveCamZ(float distance);
//
//	feng::Transform *m_cameraTransform = nullptr;
//	feng::Input *m_input = nullptr;
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
//inline void CameraController::SetCamera(feng::Transform *cameraTransform)
//{
//	m_cameraTransform = cameraTransform;
//}
//
//inline void CameraController::SetInputManager(feng::Input *inputManager)
//{
//	m_input = inputManager;
//}
