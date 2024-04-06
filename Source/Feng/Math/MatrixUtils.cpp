#include <Feng/Math/MatrixUtils.h>

#include <Feng/Math/MathUtils.h>

namespace Feng::Mat4
{
    Matrix4 MakeScale(const Vector3& scale)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m._00 = scale.coord.x;
        matrix.m._11 = scale.coord.y;
        matrix.m._22 = scale.coord.z;

        return matrix;
    }

    Matrix4 MakeRotation(const Matrix3& rotation)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m._00 = rotation.m._00;
        matrix.m._01 = rotation.m._01;
        matrix.m._02 = rotation.m._02;
        matrix.m._10 = rotation.m._10;
        matrix.m._11 = rotation.m._11;
        matrix.m._12 = rotation.m._12;
        matrix.m._20 = rotation.m._20;
        matrix.m._21 = rotation.m._21;
        matrix.m._22 = rotation.m._22;

        return matrix;
    }
    
    Matrix4 MakeRotation(const Quaternion& rotation)
    {
        return MakeRotation(rotation.ToMatrix3());
    }

    Matrix4 MakeTranslation(const Vector3& translation)
    {
        Matrix4 matrix = Matrix4::Identity;
        matrix.m._30 = translation.coord.x;
        matrix.m._31 = translation.coord.y;
        matrix.m._32 = translation.coord.z;

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

    Matrix4 MakePerspectiveProjection(float fovYDeg, float aspect, float near, float far)
    {
        Matrix4 projection = Matrix4::Zero;

        float fovYRad = Math::DegToRad(fovYDeg);
        float fovHalfTan = std::tan(fovYRad / 2.f);

        projection.m._00 = 1.f / (aspect * fovHalfTan);
        projection.m._11 = 1.f / fovHalfTan;
        projection.m._22 = - (far + near) / (far - near);
        projection.m._23 = -1.f;
        projection.m._32 = -2.f * far * near / (far - near);

        return projection;
    }

    Matrix4 MakePerspectiveProjection(float r, float l, float t, float b, float n, float f)
    {
        // http://www.songho.ca/opengl/gl_projectionmatrix.html
        Matrix4 projection = Matrix4::Zero;

        projection.m._00 = (2 * n) / (r - l);
        projection.m._02 = (r + l) / (r - l);
        projection.m._11 = 2 * n / (t - b);
        projection.m._12 = (t + b) / (t - b);
        projection.m._22 = - (f + n) / (f - n);
        projection.m._23 = -1.f;
        projection.m._32 = -2.f * f * n / (f - n);

        return projection;
    }
    
    Matrix4 MakeOrthogonalProjection(uint32_t width, uint32_t height, float near, float far)
    {
        Matrix4 projection = Matrix4::Zero;

        projection.m._00 = 2 / static_cast<float>(width);
        projection.m._11 = 2 / static_cast<float>(height);
        projection.m._22 = -2 / (far - near);
        projection.m._32 = - (far + near) / (far - near);
        projection.m._33 = 1;

        return projection;
    }

    Matrix4 MakeOrthogonalProjection(float l, float r, float b, float t, float n, float f)
    {
        // http://www.songho.ca/opengl/gl_projectionmatrix.html
        Matrix4 projection = Matrix4::Zero;

        projection.m._00 = 2 / (r - l);
        projection.m._11 = 2 / (t - b);
        projection.m._22 = -2 / (f - n);
        projection.m._30 = - (r + l) / (r - l);
        projection.m._31 = - (t + b) / (t - b);
        projection.m._32 = - (f + n) / (f - n);
        projection.m._33 = 1;

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

        rotation.m._11 = rotationCos;
        rotation.m._12 = rotationSin;
        rotation.m._21 = -rotationSin;
        rotation.m._22 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotationY(float dAngle)
    {
        Matrix3 rotation = Matrix3::Identity();

        float rad = Math::DegToRad(dAngle);
        float rotationCos = std::cos(rad);
        float rotationSin = std::sin(rad);

        rotation.m._00 = rotationCos;
        rotation.m._02 = -rotationSin;
        rotation.m._20 = rotationSin;
        rotation.m._22 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotationZ(float dAngle)
    {
        Matrix3 rotation = Matrix3::Identity();

        float rad = Math::DegToRad(dAngle);
        float rotationCos = std::cos(rad);
        float rotationSin = std::sin(rad);

        rotation.m._00 = rotationCos;
        rotation.m._01 = rotationSin;
        rotation.m._10 = -rotationSin;
        rotation.m._11 = rotationCos;

        return rotation;
    }

    Matrix3 MakeRotation(const Vector3 &dAngles)
    {
        return MakeRotation(dAngles.coord.x, dAngles.coord.y, dAngles.coord.z);
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
        return MakeRotation(dAngles.coord.x, dAngles.coord.y, dAngles.coord.z);
    }
    
    Quaternion MakeRotation(float dAngleX, float dAngleY, float dAngleZ)
    {
        return Quaternion{Vector3::OneZ, dAngleZ}
             * Quaternion{Vector3::OneY, dAngleY}
             * Quaternion{Vector3::OneX, dAngleX};
    }
}
