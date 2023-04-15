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

    Matrix4 Camera::GetViewMatrix() const
    {
        const Matrix4 myTransformMatrix = GetGlobalTransformMatrix();

        Matrix4 transformInverted;
        std::ignore = myTransformMatrix.TryInvert(transformInverted);

        // TODO: m.alekseev Ignore scale.
        return transformInverted;
    }
    
    Matrix4 Camera::GetProjectionMatrix() const
    {
        //return Mat4::MakeOrthogonalProjection(-5, 5, -5, 5, -50, 50);
        return Mat4::MakePerspectiveProjection(fovY, aspectRatio, near, far);
    }

    Matrix4 Camera::GetViewProjectionMatrix() const
    {
        Matrix4 viewMatrix = GetViewMatrix();
        Matrix4 projection = GetProjectionMatrix();

        return viewMatrix * projection;
    }
    
    Matrix4 Camera::GetViewProjectionMatrixInverse() const
    {
        const Matrix4 myTransformMatrix = GetGlobalTransformMatrix();
        
        Matrix4 projection = GetProjectionMatrix();
        Matrix4 projectionInverted;
        std::ignore = projection.TryInvert(projectionInverted);
        
        return projectionInverted * myTransformMatrix;
    }
    
    Matrix4 Camera::GetGlobalTransformMatrix() const
    {
        const Entity *myEntity = GetEntity();
        const Transform *myTransform = myEntity->GetComponent<Transform>();
        return myTransform->GetGlobalMatrix();
    }
}
