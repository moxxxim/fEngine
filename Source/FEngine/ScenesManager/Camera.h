#pragma once

#include <FEngine/Math/Matrix4.h>
#include <FEngine/ScenesManager/Component.h>

namespace feng
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
        
        Matrix4 GetProjectionMatrix() const;
        Matrix4 GetViewProjectionMatrix() const;

    private:
        static constexpr float MinFovY = 0.1f;
        static constexpr float MinAspectRatio = 0.1f;
        static constexpr float MinNear = 0.01f;

        static constexpr float DefaultFovY = 45.f;
        static constexpr float DefaultAspectRatio = 1.0f;
        static constexpr float DefaultNear = 0.1f;
        static constexpr float DefaultFar = 100.f;

        Matrix4 GetViewMatrix() const;

        float aspectRatio = DefaultAspectRatio;;
        float fovY = DefaultFovY;
        float near = DefaultNear;
        float far = DefaultFar;
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
        return near;
    }

    inline float Camera::GetFarClipPlane() const
    {
        return far;
    }
}
