#pragma once

#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Matrix4.h>
#include <Feng/Math/Quaternion.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>

namespace Feng
{
    namespace Mat4
    {
        Matrix4 MakeScale(const Vector3& scale);
        Matrix4 MakeRotation(const Matrix3& rotation);
        Matrix4 MakeRotation(const Quaternion& rotation);
        Matrix4 MakeTranslation(const Vector3& translation);
        Matrix4 MakeTransformation(const Vector3& scale, const Vector3& translation, const Matrix3& rotation);
        Matrix4 MakeTransformation(const Vector3& scale, const Vector3& translation, const Quaternion& rotation);
        Matrix4 MakePerspectiveProjection(float fovYDeg, float aspect, float near, float far);
        Matrix4 MakePerspectiveProjection(float r, float l, float t, float b, float n, float f);
        Matrix4 MakeOrthogonalProjection(uint32_t width, uint32_t height, float near, float far);
        Matrix4 MakeOrthogonalProjection(float l, float r, float b, float t, float n, float f);
    }

    namespace Mat3
    {
        Matrix3 MakeScale(const Vector3& scale);
        Matrix3 MakeRotationX(float dAngle);
        Matrix3 MakeRotationY(float dAngle);
        Matrix3 MakeRotationZ(float dAnble);
        Matrix3 MakeRotation(const Vector3 &dAngles);
        Matrix3 MakeRotation(float dAngleX, float dAngleY, float dAngleZ);
    }
    
    namespace Quat
    {
        Quaternion MakeRotation(const Vector3 &dAngles);
        Quaternion MakeRotation(float dAngleX, float dAngleY, float dAngleZ);
    }
}
