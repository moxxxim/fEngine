#include <FEngine/ScenesManager/Camera.h>

#include <FEngine/Math/MatrixUtils.h>

namespace fengine
{
    Camera::Camera()
    { }

    Matrix4 Camera::GetProjectionMatrix() const
    {
        return matutils::MakePerspectiveProjection(fovY, aspectRatio, near, far);
    }

    Matrix4 Camera::GetViewProjectionMatrix() const
    {
        Matrix4 viewMatrix = GetViewMatrix();
        Matrix4 perspectiveProjection = GetProjectionMatrix();

        return viewMatrix * perspectiveProjection;
    }

    Matrix4 Camera::GetViewMatrix() const
    {
        // inverse of cam transform
        throw "'Camera::GetViewMatrix()' is not implemented yet";
    }
}
