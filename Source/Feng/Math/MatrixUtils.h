#ifndef Matrix4Utils_hpp
#define Matrix4Utils_hpp

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>

namespace feng
{
    namespace mat4
    {
        Matrix4 MakeScale(const Vector3& scale);
        Matrix4 MakeRotation(const Matrix3& rotation);
        Matrix4 MakeTranslation(const Vector3& translation);
        Matrix4 MakeTransformation(const Vector3& scale, const Matrix3& rotation, const Vector3& translation);
        Matrix4 MakePerspectiveProjection(float fovDegY, float aspect, float near, float far);
        Matrix4 MakeOrthogonalProjection(uint32_t width, uint32_t height, float near, float far);
    }

    namespace mat3
    {
        Matrix3 MakeScale(const Vector3& scale);
        Matrix3 MakeRotationX(float degrees);
        Matrix3 MakeRotationY(float degrees);
        Matrix3 MakeRotationZ(float degrees);
        Matrix3 MakeRotation(const Vector3 &degrees);
        Matrix3 MakeRotation(float xDegrees, float yDegrees, float zDegrees);
    }
}

#endif
 
