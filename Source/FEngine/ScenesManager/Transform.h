#pragma once

namespace fengine
{
//    class Transform final
//    {
//    public:
//        struct ReferenceSystem final
//        {
//            static const Vector3 X;
//            static const Vector3 Y;
//            static const Vector3 Z;
//        };
//
//        Transform();
//        ~Transform() = default;
//
//        const Vector3& GetPosition() const;
//        void SetPosition(const Vector3& position);
//        void SetPosition(float x, float y, float z);
//        Vector3 GetRotation() const;
//        void SetRotation(float x, float y, float z);
//        void SetRotation(Vector3 rotation);
//        const Vector3& GetScale() const;
//        void SetScale(Vector3 scale);
//        void SetScale(float uniformScale);
//        const Matrix& GetTransformMatrix() const;
//        const Vector3& GetForward() const;
//        const Vector3& GetUp() const;
//        const Vector3& GetRight() const;
//        void RotateAlong(const Vector3& axis, float rad);
//        Transform *GetParent() const;
//        void SetParent(Transform *parent);
//
//        static Vector3 MatrixToEulerAngles(const Matrix& transformMatrix);
//        static Matrix MatrixFromEulerAngles(const Vector3& eulerAngles);
//        const Matrix& GetRotationMatrix() const;
//
//    private:
//        void UpdateTransformMatrix(Matrix& rotationMatrix);
//        void UpdateReferenceSystem();
//
//        Vector3 m_right;
//        Vector3 m_up;
//        Vector3 m_forward;
//        Vector3 m_position;
//        Vector3 m_scale;
//        Matrix m_rotationMatrix;
//        Matrix m_transformMatrix;
//        Transform *m_parent = nullptr;
//    };
//
//    inline const Vector3& Transform::GetPosition() const
//    {
//        return m_position;
//    }
//
//    inline const Vector3& Transform::GetScale() const
//    {
//        return m_scale;
//    }
//
//    inline const Matrix& Transform::GetTransformMatrix() const
//    {
//        return m_transformMatrix;
//    }
//
//    inline const Vector3& Transform::GetForward() const
//    {
//        return m_forward;
//    }
//
//    inline const Vector3& Transform::GetUp() const
//    {
//        return m_up;
//    }
//
//    inline const Vector3& Transform::GetRight() const
//    {
//        return m_right;
//    }
//
//    inline const Matrix& Transform::GetRotationMatrix() const
//    {
//        return m_rotationMatrix;
//    }
//
//    inline Transform *Transform::GetParent() const
//    {
//        return m_parent;
//    }
}
