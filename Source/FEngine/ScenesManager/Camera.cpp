//#include <FEngine/ScenesManager/Camera.h>
//
//#include <FEngine/ScenesManager/Transform.h>
//#include <cmath>
//
//fengine::Camera::Camera()
//{
//}
//
//Matrix fengine::Camera::GetProjectionMatrix() const
//{
//	Matrix perspectiveProjection;
//	perspectiveProjection.SetPerspective(m_fovY, m_aspectRatio, m_nearClipDistance, m_farClipDistance);
//
//	return perspectiveProjection;
//}
//
//Matrix fengine::Camera::GetViewProjectionMatrix() const
//{
//	Matrix viewMatrix = GetViewMatrix();
//	Matrix perspectiveProjection = GetProjectionMatrix();
//
//	return viewMatrix * perspectiveProjection;
//}
//
//void fengine::Camera::Serialize(fengine::SerializationNode& node) const
//{
//	node.SetName("Camera");
//	node.SetFloat("AspectRatio", m_aspectRatio);
//	node.SetFloat("FovY", m_fovY);
//	node.SetFloat("NearClipDistance", m_nearClipDistance);
//	node.SetFloat("FarClipDistance", m_farClipDistance);
//}
//
//void fengine::Camera::Deserialize(const fengine::SerializationNode& node)
//{
//	node.TryGetFloat("AspectRatio", m_aspectRatio);
//	node.TryGetFloat("FovY", m_fovY);
//	node.TryGetFloat("NearClipDistance", m_nearClipDistance);
//	node.TryGetFloat("FarClipDistance", m_farClipDistance);
//}
//
//Matrix fengine::Camera::GetViewMatrix() const
//{
//	fengine::Transform *transform = GetTransform();
//
//	const Vector3& position = transform->GetPosition();
//	const Matrix& rotationMatrix = transform->GetRotationMatrix();
//	Matrix positionMatrixInvers;
//	positionMatrixInvers.SetTranslation(-position.x, -position.y, -position.z);
//	Matrix rotationMatrixInverse = Matrix(rotationMatrix);
//	rotationMatrixInverse.Transpose();
//
//	return positionMatrixInvers * rotationMatrixInverse;
//}
