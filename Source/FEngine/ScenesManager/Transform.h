#pragma once

#include <FEngine/Math/Matrix3.h>
#include <FEngine/Math/Matrix4.h>
#include <FEngine/Math/Vector3.h>

namespace fengine
{
    class Transform final
    {
    public:
        Transform();
        ~Transform() = default;

        const Vector3& GetPosition() const;
        void SetPosition(const Vector3& aPosition);
        void SetPosition(float x, float y, float z);

        const Matrix3& GetRotation() const;
        void SetRotation(const Matrix3& aRotation);
        Vector3 GetRotationEuler() const;
        void SetRotationEuler(const Vector3& euler);
        void SetRotationEuler(float x, float y, float z);

        const Vector3& GetScale() const;
        void SetScale(const Vector3& aScale);
        void SetScale(float x, float y, float z);
        void SetScale(float uniformScale);

        Vector3 GetForward() const;
        Vector3 GetUp() const;
        Vector3 GetRight() const;
        Matrix4 GetGlobalMatrix() const;

        void Move(const Vector3& translation);
        void Move(float x, float y, float z);
        void Rotate(const Vector3& euler);
        void Rotate(float x, float y, float z);

    private:
        Matrix3 rotation;
        Vector3 position;
        Vector3 scale;
    };
}

namespace fengine
{
    inline const Vector3& Transform::GetPosition() const
    {
        return position;
    }

    inline void Transform::SetPosition(const Vector3& aPosition)
    {
        position = aPosition;
    }

    inline void Transform::SetPosition(float x, float y, float z)
    {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    inline const Matrix3& Transform::GetRotation() const
    {
        return rotation;
    }

    inline void Transform::SetRotation(const Matrix3& aRotation)
    {
        rotation = aRotation;
    }

    inline const Vector3& Transform::GetScale() const
    {
        return scale;
    }

    inline void Transform::SetScale(const Vector3& aScale)
    {
        scale = aScale;
    }

    inline void Transform::SetScale(float x, float y, float z)
    {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }

    inline void Transform::SetScale(float uniformScale)
    {
        scale.x = uniformScale;
        scale.y = uniformScale;
        scale.z = uniformScale;
    }

    inline void Transform::Move(const Vector3& translation)
    {
        position += translation;
    }

    inline void Transform::Move(float x, float y, float z)
    {
        position.x += x;
        position.y += y;
        position.z += z;
    }
}
