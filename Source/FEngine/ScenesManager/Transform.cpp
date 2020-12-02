#include <FEngine/ScenesManager/Transform.h>

#include <FEngine/Math/MatrixUtils.h>

namespace fengine
{
    Transform::Transform()
        : rotation { Matrix3::Identity }
        , position{ Vector3::Zero }
        , scale{ Vector3::One }
    { }

    Vector3 Transform::GetRotationEuler() const
    {
        throw "'Transform::GetRotationEuler()' not implemented yet.";
    }

    void Transform::SetRotationEuler(float x, float y, float z)
    {
        throw "'Transform::SetRotationEuler()' not implemented yet.";
    }

    void Transform::SetRotationEuler(const Vector3& angles)
    {
        throw "'Transform::SetRotationEuler()' not implemented yet.";
    }

    Vector3 Transform::GetForward() const
    {
        throw "'Transform::GetForward()' not implemented yet.";
    }

    Vector3 Transform::GetUp() const
    {
        throw "'Transform::GetUp()' not implemented yet.";
    }

    Vector3 Transform::GetRight() const
    {
        throw "'Transform::GetRight()' not implemented yet.";
    }

    Matrix4 Transform::GetGlobalMatrix() const
    {
        return matutils::MakeTransformation(scale, rotation, position);
    }
}
