#include <FEngine/ScenesManager/Transform.h>

#include <FEngine/Math/MatrixUtils.h>

namespace fengine
{
    Transform::Transform()
        : rotation { Matrix3::Identity }
        , position { Vector3::Zero }
        , scale { Vector3::One }
    { }

    Vector3 Transform::GetEuler() const
    {
        throw "'Transform::GetRotationEuler()' not implemented yet.";
    }

void Transform::SetEuler(float x, float y, float z, eSpace space /*= eSpace::World*/)
    {
        rotation = mat3::MakeRotation(x, y, z);
    }

    void Transform::SetEuler(const Vector3& euler, eSpace space /*= eSpace::World*/)
    {
        rotation = mat3::MakeRotation(euler);
    }

    Vector3 Transform::GetForward() const
    {
        return -Vector3::OneZ * mat3::MakeScale(scale) * rotation;
    }

    Vector3 Transform::GetUp() const
    {
        return Vector3::OneY * mat3::MakeScale(scale) * rotation;
    }

    Vector3 Transform::GetRight() const
    {
        return Vector3::OneX * mat3::MakeScale(scale) * rotation;
    }

    Matrix4 Transform::GetGlobalMatrix() const
    {
        return mat4::MakeTransformation(scale, rotation, position);
    }

    void Transform::Rotate(const Vector3& euler)
    {
        rotation *= mat3::MakeRotation(euler);
    }

    void Transform::Rotate(float x, float y, float z)
    {
        rotation *= mat3::MakeRotation(x, y, z);
    }
}
