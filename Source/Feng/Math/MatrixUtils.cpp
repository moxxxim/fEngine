#include <Feng/Math/MatrixUtils.h>

#include <Feng/Math/MathUtils.h>

namespace Feng::Mat4
{
    Matrix4 MakeScale(const Vector3& scale)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m00 = scale.x;
        matrix.m11 = scale.y;
        matrix.m22 = scale.z;

        return matrix;
    }

    Matrix4 MakeRotation(const Matrix3& rotation)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m00 = rotation.m00;
        matrix.m01 = rotation.m01;
        matrix.m02 = rotation.m02;

        matrix.m10 = rotation.m10;
        matrix.m11 = rotation.m11;
        matrix.m12 = rotation.m12;

        matrix.m20 = rotation.m20;
        matrix.m21 = rotation.m21;
        matrix.m22 = rotation.m22;

        return matrix;
    }
    
    Matrix4 MakeRotation(const Quaternion& rotation)
    {
        return MakeRotation(rotation.ToMatrix3());
    }

    Matrix4 MakeTranslation(const Vector3& translation)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m30 = translation.x;
        matrix.m31 = translation.y;
        matrix.m32 = translation.z;

        return matrix;
    }

    Matrix4 MakeTransformation(const Vector3& scale, const Vector3& translation, const Matrix3& rotation)
    {
        // TODO: optimize.

        Matrix4 scaleMat = MakeScale(scale);
        Matrix4 rotateMat = MakeRotation(rotation);
        Matrix4 translateMat = MakeTranslation(translation);

        return scaleMat * rotateMat * translateMat;
    }
    
    Matrix4 MakeTransformation(const Vector3& scale, const Vector3& translation, const Quaternion& rotation)
    {
        // TODO: optimize.
        return MakeTransformation(scale, translation, rotation.ToMatrix3());
    }

    Matrix4 MakePerspectiveProjection(float fovYDeg, float aspect, float near, float far, bool alongZ /*= false*/)
    {
        Matrix4 projection = Matrix4::Zero;

        float fovYRad = Math::DegToRad(fovYDeg);
        float fovHalfTan = std::tan(fovYRad / 2.f);

        projection.m00 = 1.f / (aspect * fovHalfTan);
        projection.m11 = 1.f / fovHalfTan;
        projection.m22 = - (far + near) / (far - near);
        projection.m23 = -1.f;
        projection.m32 = -2.f * far * near / (far - near);

        return projection;
    }

    Matrix4 MakeOrthogonalProjection(uint32_t width, uint32_t height, float near, float far, bool alongZ /*= false*/)
    {
        float xSign = alongZ ? -1 : 1;
        if(alongZ)
        {
            far *= -1;
            near *= -1;
        }

        Matrix4 projection = Matrix4::Zero;

        projection.m00 = xSign / static_cast<float>(width);
        projection.m11 = 1 / static_cast<float>(height);
        projection.m22 = -2 / (far - near);
        projection.m33 = 1;
        projection.m32 = - (far + near) / (far - near);

        return projection;
    }
}

namespace Feng::Mat3
{
    Matrix3 MakeScale(const Vector3& scale)
    {
        return Matrix3::Diag(scale);
    }

    Matrix3 MakeRotationX(float dAngle)
    {
        Matrix3 rotation = Matrix3::Identity();

        float rad = Math::DegToRad(dAngle);
        float rotationCos = std::cos(rad);
        float rotationSin = std::sin(rad);

        rotation.m11 = rotationCos;
        rotation.m12 = rotationSin;
        rotation.m21 = -rotationSin;
        rotation.m22 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotationY(float dAngle)
    {
        Matrix3 rotation = Matrix3::Identity();

        float rad = Math::DegToRad(dAngle);
        float rotationCos = std::cos(rad);
        float rotationSin = std::sin(rad);

        rotation.m00 = rotationCos;
        rotation.m02 = -rotationSin;
        rotation.m20 = rotationSin;
        rotation.m22 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotationZ(float dAngle)
    {
        Matrix3 rotation = Matrix3::Identity();

        float rad = Math::DegToRad(dAngle);
        float rotationCos = std::cos(rad);
        float rotationSin = std::sin(rad);

        rotation.m00 = rotationCos;
        rotation.m01 = rotationSin;
        rotation.m10 = -rotationSin;
        rotation.m11 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotation(const Vector3 &dAngles)
    {
        return MakeRotation(dAngles.x, dAngles.y, dAngles.z);
    }

    Matrix3 MakeRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        // TODO: optimize
        return MakeRotationZ(dAngleZ) * MakeRotationY(dAngleY) * MakeRotationX(dAngleX);
    }
}

namespace Feng::Quat
{
    Quaternion MakeRotation(const Vector3 &dAngles)
    {
        return MakeRotation(dAngles.x, dAngles.y, dAngles.z);
    }
    
    Quaternion MakeRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        return Quaternion{Vector3::OneZ, dAngleZ}
             * Quaternion{Vector3::OneY, dAngleY}
             * Quaternion{Vector3::OneX, dAngleX};
    }
}
