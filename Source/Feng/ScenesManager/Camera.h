#pragma once

#include <Feng/Math/Matrix4.h>
#include <Feng/ScenesManager/Component.h>

namespace Feng
{
    class Camera final : public Component
    {
    public:
        Camera() = default;

        float GetFovY() const;
	    void SetFovY(float fov);

	    float GetAspectRatio() const;
	    void SetAspectRatio(float aspectRatio);

	    float GetNearClipPlane() const;
        void SetNearClipPlane(float aNear);

	    float GetFarClipPlane() const;
	    void SetFarClipPlane(float aFar);
        
        Matrix4 GetViewMatrix() const;
        Matrix4 GetProjectionMatrix() const;
        Matrix4 GetViewProjectionMatrix() const;
        Matrix4 GetViewProjectionMatrixInverse() const;

    private:
        static constexpr float MinFovY = 0.1f;
        static constexpr float MinAspectRatio = 0.1f;
        static constexpr float MinNear = 0.01f;

        static constexpr float DefaultFovY = 30.f;
        static constexpr float DefaultAspectRatio = 1.0f;
        static constexpr float DefaultNear = 1.f;
        static constexpr float DefaultFar = 10.f;

        Matrix4 GetGlobalTransformMatrix() const;

        float aspectRatio = DefaultAspectRatio;
        float fovY = DefaultFovY;
        float nearPlane = DefaultNear;
        float farPlane = DefaultFar;
    };

    inline float Camera::GetFovY() const
    {
        return fovY;
    }

    inline float Camera::GetAspectRatio() const
    {
        return aspectRatio;
    }

    inline float Camera::GetNearClipPlane() const
    {
        return nearPlane;
    }

    inline float Camera::GetFarClipPlane() const
    {
        return farPlane;
    }
}
