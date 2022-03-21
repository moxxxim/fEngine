#include <Feng/ScenesManager/Transform.h>

#include <Feng/Math/MatrixUtils.h>

namespace Feng
{
    Transform::Transform()
        : rotation { Quaternion::Identity() }
        , position { Vector3::Zero }
        , scale { Vector3::One }
    { }

    void Transform::SetRotation(const Quaternion& aRotation, eSpace space /*= eSpace::World*/)
    {
        rotation = (space == eSpace::World) ? aRotation : aRotation * rotation;
    }

    Vector3 Transform::GetEuler(eSpace space /*= eSpace::World*/) const
    {
        throw "'Transform::GetRotationEuler()' not implemented yet.";
    }

    void Transform::SetEuler(float x, float y, float z, eSpace space /*= eSpace::World*/)
    {
        SetRotation(Quat::MakeRotation(x, y, z), space);
    }

    void Transform::SetEuler(const Vector3& euler, eSpace space /*= eSpace::World*/)
    {
        SetEuler(euler.x, euler.y, euler.z, space);
    }

    Vector3 Transform::GetForward() const
    {
        return -Vector3::OneZ * rotation;
    }

    Vector3 Transform::GetUp() const
    {
        return Vector3::OneY * rotation;
    }

    Vector3 Transform::GetRight() const
    {
        return Vector3::OneX * rotation;
    }

    Matrix4 Transform::GetGlobalMatrix() const
    {
        return Mat4::MakeTransformation(scale, position, rotation);
    }

    void Transform::Rotate(float x, float y, float z, eSpace space /*= eSpace::World*/)
    {
        Quaternion change = Quat::MakeRotation(x, y, z);
        rotation = (space == eSpace::World) ? rotation * change : change * rotation;
    }
    
    void Transform::Rotate(const Vector3& euler, eSpace space /*= eSpace::World*/)
    {
        Rotate(euler.x, euler.y, euler.z);
    }
    
    void Transform::Rotate(const Vector3& axisNormalized, float dAngle, eSpace space /*= eSpace::World*/)
    {
        Quaternion change {axisNormalized, dAngle};
        rotation = (space == eSpace::World) ? rotation * change : change * rotation;
    }
}
