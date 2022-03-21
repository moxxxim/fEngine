#pragma once

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Quaternion.h>
#include <Feng/ScenesManager/Component.h>
#include <Feng/ScenesManager/Space.h>

namespace Feng
{
    class Transform final : public Component
    {
    public:
        Transform();
        ~Transform() = default;

        const Vector3& GetPosition(eSpace space = eSpace::World) const;
        void SetPosition(const Vector3& aPosition, eSpace space = eSpace::World);
        void SetPosition(float x, float y, float z, eSpace space = eSpace::World);

        const Quaternion& GetRotation(eSpace space = eSpace::World) const;
        void SetRotation(const Quaternion& aRotation, eSpace space = eSpace::World);
        Vector3 GetEuler(eSpace space = eSpace::World) const;
        void SetEuler(float x, float y, float z, eSpace space = eSpace::World);
        void SetEuler(const Vector3& euler, eSpace space = eSpace::World);

        const Vector3& GetScale() const;
        void SetScale(float x, float y, float z);
        void SetScale(const Vector3& aScale);
        void SetScale(float uniformScale);

        Vector3 GetForward() const;
        Vector3 GetUp() const;
        Vector3 GetRight() const;
        Matrix4 GetGlobalMatrix() const;

        void Move(const Vector3& translation);
        void Move(float x, float y, float z);
        void Rotate(float x, float y, float z, eSpace space = eSpace::World);
        void Rotate(const Vector3& euler, eSpace space = eSpace::World);
        void Rotate(const Vector3& axisNormalized, float dAngle, eSpace space = eSpace::World);

    private:
        Quaternion rotation;
        Vector3 position;
        Vector3 scale;
    };
}

namespace Feng
{
    inline const Vector3& Transform::GetPosition(eSpace space /*= eSpace::World*/) const
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

    inline const Quaternion& Transform::GetRotation(eSpace space /*= eSpace::World*/) const
    {
        return rotation;
    }

    inline const Vector3& Transform::GetScale() const
    {
        return scale;
    }

    inline void Transform::SetScale(float x, float y, float z)
    {
        scale.x = x;
        scale.y = y;
        scale.z = z;
    }
    
    inline void Transform::SetScale(const Vector3& aScale)
    {
        scale = aScale;
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
