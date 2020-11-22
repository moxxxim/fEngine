//#pragma once
//
//#include <FEngine/ScenesManager/GameObjectComponent.h>
//
//namespace fengine
//{
//    class Camera final: public GameObjectComponent
//    {
//    public:
//        Camera();
//
//        float GetFovY() const;
//	    void SetFovY(float fovY);
//	    float GetAspectRatio() const;
//	    void SetAspectRatio(float aspectRatio);
//	    float GetNearClipPlane() const;
//        void SetNearClipPlane(float nearClipDistance);
//	    float GetFarClipPlane() const;
//	    void SetFarClipPlane(float farClipDistance);
//        Matrix GetProjectionMatrix() const;
//        Matrix GetViewProjectionMatrix() const;
//
//        void Serialize(SerializationNode& node) const override;
//        void Deserialize(const SerializationNode& node) override;
//
//private:
//        Matrix GetViewMatrix() const;
//
//        float m_aspectRatio = 1.0f;
//        float m_fovY = 90.0f;
//        float m_nearClipDistance = 0.1f;
//        float m_farClipDistance = 90.0f;
//    };
//
//    inline float Camera::GetFovY() const
//    {
//        return m_fovY;
//    }
//
//    inline void Camera::SetFovY(float fov)
//    {
//        m_fovY = fov;
//    }
//
//    inline float Camera::GetAspectRatio() const
//    {
//        return m_aspectRatio;
//    }
//
//    inline void Camera::SetAspectRatio(float aspectRatio)
//    {
//        m_aspectRatio = aspectRatio;
//    }
//
//    inline float Camera::GetNearClipPlane() const
//    {
//        return m_nearClipDistance;
//    }
//
//    inline void Camera::SetNearClipPlane(float nearClipDistance)
//    {
//        m_nearClipDistance = nearClipDistance;
//    }
//
//    inline float Camera::GetFarClipPlane() const
//    {
//        return m_farClipDistance;
//    }
//
//    inline void Camera::SetFarClipPlane(float farClipDistance)
//    {
//        m_farClipDistance = farClipDistance;
//    }
//}
