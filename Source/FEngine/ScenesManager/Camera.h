#pragma once

#include <FEngine/Math/Matrix4.h>

namespace fengine
{
    class Camera final
    {
    public:
        Camera();

        float GetFovY() const;
	    void SetFovY(float fov);
	    float GetAspectRatio() const;
	    void SetAspectRatio(float aspectRatio);
	    float GetNearClipPlane() const;
        void SetNearClipPlane(float aNear);
	    float GetFarClipPlane() const;
	    void SetFarClipPlane(float aFar);
        Matrix4 GetProjectionMatrix() const;
        Matrix4 GetViewProjectionMatrix() const;

private:
        Matrix4 GetViewMatrix() const;

        float aspectRatio = 1.0f;
        float fovY = 90.0f;
        float near = 0.1f;
        float far = 90.0f;
    };

    inline float Camera::GetFovY() const
    {
        return fovY;
    }

    inline void Camera::SetFovY(float fov)
    {
        fovY = fov;
    }

    inline float Camera::GetAspectRatio() const
    {
        return aspectRatio;
    }

    inline void Camera::SetAspectRatio(float aAspectRatio)
    {
        aspectRatio = aAspectRatio;
    }

    inline float Camera::GetNearClipPlane() const
    {
        return near;
    }

    inline void Camera::SetNearClipPlane(float aNear)
    {
        near = aNear;
    }

    inline float Camera::GetFarClipPlane() const
    {
        return far;
    }

    inline void Camera::SetFarClipPlane(float aFar)
    {
        far = aFar;
    }
}
