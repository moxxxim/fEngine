#pragma once

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector3.h>
#include <Feng/ScenesManager/Component.h>
#include <Feng/ScenesManager/Space.h>

namespace feng
{
    class Transform final : public Component
    {
    public:
        Transform();
        ~Transform() = default;

        const Vector3& GetPosition() const;
        void SetPosition(const Vector3& aPosition, eSpace space = eSpace::World);
        void SetPosition(float x, float y, float z, eSpace space = eSpace::World);

        const Matrix3& GetRotation(eSpace space = eSpace::World) const;
        void SetRotation(const Matrix3& aRotation, eSpace space = eSpace::World);
        Vector3 GetEuler(eSpace space = eSpace::World) const;
        void SetEuler(const Vector3& euler, eSpace space = eSpace::World);
        void SetEuler(float x, float y, float z, eSpace space = eSpace::World);

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

namespace feng
{
    inline const Vector3& Transform::GetPosition() const
    {
        return position;
    }

    inline void Transform::SetPosition(const Vector3& aPosition, eSpace space /*= eSpace::World*/)
    {
        position = aPosition;
    }

    inline void Transform::SetPosition(float x, float y, float z, eSpace space /*= eSpace::World*/)
    {
        position.x = x;
        position.y = y;
        position.z = z;
    }

    inline const Matrix3& Transform::GetRotation(eSpace space /*= eSpace::World*/) const
    {
        return rotation;
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
