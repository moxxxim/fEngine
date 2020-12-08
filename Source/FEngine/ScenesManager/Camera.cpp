#include <FEngine/ScenesManager/Camera.h>

#include <FEngine/Math/MatrixUtils.h>
#include <FEngine/ScenesManager/Entity.h>
#include <FEngine/ScenesManager/Transform.h>

namespace feng
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
        return mat4::MakePerspectiveProjection(fovY, aspectRatio, near, far);
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
        const Vector3 myScale = myTransform->GetScale();

        Matrix4 transformInverted;
        myTransformMatrix.TryInvert(transformInverted);

        // TODO: m.alekseev Ignore scale.
        return transformInverted;
    }
}
