#include <FEngine/Math/MatrixUtils.h>

namespace fengine
{
    namespace matutils
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
            Matrix4 matrix = Matrix4::Zero;
            matrix.m30 = translation.x;
            matrix.m31 = translation.y;
            matrix.m32 = translation.z;
            matrix.m33 = 1;

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
    }
}
