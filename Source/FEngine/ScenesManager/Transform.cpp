//#include "stdafx.h"
//
//#include "Transform.h"
//#include <cmath>
//
//const Vector3 fengine::Transform::ReferenceSystem::X = Vector3(1, 0, 0);
//const Vector3 fengine::Transform::ReferenceSystem::Y = Vector3(0, 1, 0);
//const Vector3 fengine::Transform::ReferenceSystem::Z = Vector3(0, 0, 1);
//
//fengine::Transform::Transform() :
//	m_position(0, 0, 0),
//	m_scale(1, 1, 1),
//	m_right(ReferenceSystem::X),
//	m_up(ReferenceSystem::Y),
//	m_forward(ReferenceSystem::Z)
//{
//	m_rotationMatrix.SetIdentity();
//	m_transformMatrix.SetIdentity();
//}
//
//void fengine::Transform::SetPosition(const Vector3& position)
//{
//	SetPosition(position.x, position.y, position.z);
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//void fengine::Transform::SetPosition(float x, float y, float z)
//{
//	Vector3 xOrt = ReferenceSystem::X;
//	Vector3 yOrt = ReferenceSystem::Y;
//	Vector3 zOrt = ReferenceSystem::Z;
//
//	m_position = xOrt * x + yOrt * y + zOrt * z;
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//Vector3 fengine::Transform::GetRotation() const
//{
//	return MatrixToEulerAngles(m_rotationMatrix);
//}
//
//void fengine::Transform::SetRotation(float x, float y, float z)
//{
//	SetRotation(Vector3(x, y, z));
//}
//
//void fengine::Transform::SetRotation(Vector3 rotationRad)
//{
//	m_rotationMatrix = MatrixFromEulerAngles(rotationRad);
//
//	UpdateReferenceSystem();
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//void fengine::Transform::SetScale(Vector3 scale)
//{
//	m_scale = scale;
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//void fengine::Transform::SetScale(float uniformScale)
//{
//	m_scale = Vector3(uniformScale, uniformScale, uniformScale);
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//void fengine::Transform::RotateAlong(const Vector3& axis, float rad)
//{
//	Matrix rotationDelta;
//	rotationDelta.SetRotationAngleAxis(rad, axis.x, axis.y, axis.z);
//	m_rotationMatrix = rotationDelta * m_rotationMatrix;
//
//	UpdateReferenceSystem();
//	UpdateTransformMatrix(m_rotationMatrix);
//}
//
//Matrix fengine::Transform::MatrixFromEulerAngles(const Vector3& eulerAngles)
//{
//	Matrix rotationX;
//	Matrix rotationY;
//	Matrix rotationZ;
//
//	rotationX.SetRotationX(eulerAngles.x);
//	rotationY.SetRotationY(eulerAngles.y);
//	rotationZ.SetRotationZ(eulerAngles.z);
//
//	return rotationZ * rotationX * rotationY;
//}
//
//Vector3 fengine::Transform::MatrixToEulerAngles(const Matrix& rotationMatrix)
//{
//	Vector3 eulerAngles;
//
//	if (rotationMatrix.m[2][1] == 0)
//	{
//		eulerAngles.x = 0;
//		eulerAngles.y = asin(rotationMatrix.m[2][0]);
//		eulerAngles.z = asin(rotationMatrix.m[0][1]);
//	}
//	else
//	{
//		eulerAngles.x = asin(-rotationMatrix.m[2][1]);
//		float xCos = sqrt(1 - (rotationMatrix.m[2][1] * rotationMatrix.m[3][2]));
//
//		eulerAngles.y = asin(rotationMatrix.m[2][0] / xCos);
//		eulerAngles.z = asin(rotationMatrix.m[0][1] / xCos);
//	}
//
//	return eulerAngles;
//}
//
//void fengine::Transform::UpdateTransformMatrix(Matrix& rotationMatrix)
//{
//	Matrix positionMatrix;
//	positionMatrix.SetTranslation(m_position);
//	Matrix scaleMatrix;
//	scaleMatrix.SetScale(m_scale);
//
//	m_transformMatrix = scaleMatrix * rotationMatrix * positionMatrix;
//}
//
//void fengine::Transform::UpdateReferenceSystem()
//{
//	Matrix scaleMatrix;
//	Vector3 scaleSigns = Vector3(m_scale.x > 0 ? 1 : -1, m_scale.y > 0 ? 1 : -1, m_scale.z > 0 ? 1 : -1);
//	scaleMatrix.SetScale(scaleSigns);
//
//	Vector4 forward = scaleMatrix * m_rotationMatrix *Vector4(ReferenceSystem::Z.x, ReferenceSystem::Z.y, ReferenceSystem::Z.z, 0);
//	m_forward = Vector3(forward.x, forward.y, forward.z);
//
//	Vector4 up = scaleMatrix * m_rotationMatrix * Vector4(ReferenceSystem::Y.x, ReferenceSystem::Y.y, ReferenceSystem::Y.z, 0);
//	m_up = Vector3(up.x, up.y, up.z);
//
//	Vector4 right = scaleMatrix * m_rotationMatrix * Vector4(ReferenceSystem::X.x, ReferenceSystem::X.y, ReferenceSystem::X.z, 0);
//	m_right = Vector3(right.x, right.y, right.z);
//}
//
//void fengine::Transform::SetParent(Transform *parent)
//{
//	m_parent = parent;
//}
