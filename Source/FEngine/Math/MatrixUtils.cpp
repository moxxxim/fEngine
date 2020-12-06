#include <FEngine/Math/MatrixUtils.h>

#include <math.h>

namespace fengine
{
    namespace SMath
    {
        constexpr float DegToRad(float degrees)
        {
            return degrees * M_PI / 180.f;
        }

        constexpr float RadToDeg(float rad)
        {
            return rad * 180.f / M_PI;
        }
    }

    namespace mat4
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

        Matrix4 MakeTranslation(const Vector3& translation)
        {
            Matrix4 matrix = Matrix4::Identity;
            matrix.m30 = translation.x;
            matrix.m31 = translation.y;
            matrix.m32 = translation.z;

            return matrix;
        }

        Matrix4 MakeTransformation(const Vector3& scale, const Matrix3& rotation, const Vector3& translation)
        {
            // TODO: optimize.

            Matrix4 scaleMat = MakeScale(scale);
            Matrix4 rotateMat = MakeRotation(rotation);
            Matrix4 translateMat = MakeTranslation(translation);

            return scaleMat * rotateMat * translateMat;
        }

        Matrix4 MakePerspectiveProjection(float fovDegY, float aspect, float near, float far)
        {
            Matrix4 projection = Matrix4::Zero;

            float rad = SMath::DegToRad(fovDegY);
            float fovHalfTan = std::tan(rad / 2.f);

            projection.m00 = 1.f / (aspect * fovHalfTan);
            projection.m11 = 1.f / fovHalfTan;
            projection.m22 = - (far + near) / (far - near);
            projection.m23 = -1.f;
            projection.m32 = -2.f * far * near / (far - near);

            return projection;
        }
    }

    namespace mat3
    {
        Matrix3 MakeScale(const Vector3& scale)
        {
            return Matrix3::Diag(scale);
        }

        Matrix3 MakeRotationX(float degrees)
        {
            Matrix3 rotation = Matrix3::Identity;

            float rad = SMath::DegToRad(degrees);
            float rotationCos = std::cos(rad);
            float rotationSin = std::sin(rad);

            rotation.m11 = rotationCos;
            rotation.m12 = rotationSin;
            rotation.m21 = -rotationSin;
            rotation.m22 = rotationCos;

            return rotation;
        }

        Matrix3 MakeRotationY(float degrees)
        {
            Matrix3 rotation = Matrix3::Identity;

            float rad = SMath::DegToRad(degrees);
            float rotationCos = std::cos(rad);
            float rotationSin = std::sin(rad);

            rotation.m00 = rotationCos;
            rotation.m02 = rotationSin;
            rotation.m20 = -rotationSin;
            rotation.m22 = rotationCos;

            return rotation;
        }

        Matrix3 MakeRotationZ(float degrees)
        {
            Matrix3 rotation = Matrix3::Identity;

            float rad = SMath::DegToRad(degrees);
            float rotationCos = std::cos(rad);
            float rotationSin = std::sin(rad);

            rotation.m00 = rotationCos;
            rotation.m01 = rotationSin;
            rotation.m10 = -rotationSin;
            rotation.m11 = rotationCos;

            return rotation;
        }

        Matrix3 MakeRotation(const Vector3 &degrees)
        {
            return MakeRotation(degrees.x, degrees.y, degrees.z);
        }

        Matrix3 MakeRotation(float xDegrees, float yDegrees, float zDegrees)
        {
            // TODO: optimize
            return MakeRotationZ(zDegrees) * MakeRotationY(yDegrees) * MakeRotationX(xDegrees);
        }
    }
}
