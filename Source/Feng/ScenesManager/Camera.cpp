#include <Feng/ScenesManager/Camera.h>

#include <Feng/Math/MatrixUtils.h>
#include <Feng/ScenesManager/Entity.h>
#include <Feng/ScenesManager/Transform.h>

namespace Feng
{
    void Camera::SetFovY(float fov)
    {
        // TODO: m.alekseev Validate.
        fovY = fov;
    }

    void Camera::SetAspectRatio(float aAspectRatio)
    {
        // TODO: m.alekseev Validate.
        aspectRatio = aAspectRatio;
    }

    void Camera::SetNearClipPlane(float aNear)
    {
        // TODO: m.alekseev Validate.
        near = aNear;
    }

    void Camera::SetFarClipPlane(float aFar)
    {
        // TODO: m.alekseev Validate.
        far = aFar;
    }

    Matrix4 Camera::GetProjectionMatrix() const
    {
        return Mat4::MakePerspectiveProjection(fovY, aspectRatio, near, far);
    }

    Matrix4 Camera::GetViewProjectionMatrix() const
    {
        Matrix4 viewMatrix = GetViewMatrix();
        Matrix4 perspectiveProjection = GetProjectionMatrix();

        return viewMatrix * perspectiveProjection;
    }

    Matrix4 Camera::GetViewMatrix() const
    {
        const Entity *myEntity = GetEntity();
        const Transform *myTransform = myEntity->GetComponent<Transform>();
        const Matrix4 myTransformMatrix = myTransform->GetGlobalMatrix();

        Matrix4 transformInverted;
        std::ignore = myTransformMatrix.TryInvert(transformInverted);

        // TODO: m.alekseev Ignore scale.
        return transformInverted;
    }
}
