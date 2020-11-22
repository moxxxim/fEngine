//#include <Classes/Scripts/CameraController.h>
//
//void CameraController::Start()
//{
////	m_input = fengine::Input::GetInstance();
////	m_cameraTransform = GetTransform();
//}
//
//void CameraController::Update(float deltaTime)
//{
////	if (m_input && m_cameraTransform)
////	{
////		if (m_input->IsKeyPressed())
////		{
////			ApplyCamAction(m_input->GetPressedKey(), deltaTime);
////		}
////	}
//}
//
//void CameraController::Serialize(fengine::SerializationNode& node) const
//{
//	node.SetName("CameraController");
//	node.SetFloat("MoveSpeed", m_moveSpeed);
//	node.SetFloat("RotateSpeed", m_rotateSpeed);
//}
//
//void CameraController::Deserialize(const fengine::SerializationNode& node)
//{
//	node.TryGetFloat("MoveSpeed", m_moveSpeed);
//	node.TryGetFloat("RotateSpeed", m_rotateSpeed);
//}
//
//void CameraController::ApplyCamAction(unsigned char key, float delta)
//{
//	switch (key)
//	{
//		case k_camMoveForwardKey:
//		{
//			MoveCamZ(-delta * m_moveSpeed);
//		}
//		break;
//
//		case k_camMoveBackwardKey:
//		{
//			MoveCamZ(delta * m_moveSpeed);
//		}
//		break;
//
//		case k_camMoveRightKey:
//		{
//			MoveCamX(delta * m_moveSpeed);
//		}
//		break;
//
//		case k_camMoveLeftKey:
//		{
//			MoveCamX(-delta * m_moveSpeed);
//		}
//		break;
//
//		case k_camPanRightKey:
//		{
//			PanCam(delta * m_rotateSpeed);
//		}
//		break;
//
//		case k_camPanLeftKey:
//		{
//			PanCam(-delta * m_rotateSpeed);
//		}
//		break;
//
//		case k_camTiltUpKey:
//		{
//			TiltCam(-delta * m_rotateSpeed);
//		}
//		break;
//
//		case k_camTiltDownKey:
//		{
//			TiltCam(delta * m_rotateSpeed);
//		}
//		break;
//	}
//}
//
//void CameraController::TiltCam(float rad)
//{
////	Vector3 camRight = m_cameraTransform->GetRight();
////	m_cameraTransform->RotateAlong(camRight, rad);
//}
//
//void CameraController::PanCam(float rad)
//{
////	m_cameraTransform->RotateAlong(Vector3(0.0, 1.0, 0.0), rad);
//}
//
//void CameraController::MoveCamX(float distance)
//{
////	Vector3 camPosition = m_cameraTransform->GetPosition();
////	Vector3 camRight = m_cameraTransform->GetRight();
////	m_cameraTransform->SetPosition(camPosition + camRight * distance);
//}
//
//void CameraController::MoveCamZ(float distance)
//{
////	Vector3 camPosition = m_cameraTransform->GetPosition();
////	Vector3 camForward = m_cameraTransform->GetForward();
////	m_cameraTransform->SetPosition(camPosition + camForward * distance);
//}
