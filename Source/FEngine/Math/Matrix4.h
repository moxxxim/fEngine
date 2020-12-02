#ifndef Matrix4_hpp
#define Matrix4_hpp

#include <FEngine/Math/Vector4.h>

namespace fengine
{
    class Matrix4 final
    {
    public:
        Matrix4() = default;
        Matrix4(const Matrix4&) = default;
        Matrix4(Matrix4&&) = default;

        friend Matrix4 operator * (const Matrix4& a, const Matrix4& b);
        friend Vector4 operator * (const Vector4& a, const Matrix4& b);

        static const Matrix4 Zero;
        static const Matrix4 Identity;

        union
        {
            float data[16];
            struct
            {
                float m00;
                float m01;
                float m02;
                float m03;

                float m10;
                float m11;
                float m12;
                float m13;

                float m20;
                float m21;
                float m22;
                float m23;

                float m30;
                float m31;
                float m32;
                float m33;
            };
        };
    };
}

namespace fengine
{
    inline Matrix4 operator * (const Matrix4& a, const Matrix4& b)
    {
        Matrix4 result;

        result.m00 = (a.m00 * b.m00) + (a.m01 * b.m10) + (a.m02 * b.m20) + (a.m03 * b.m30);
        result.m01 = (a.m00 * b.m01) + (a.m01 * b.m11) + (a.m02 * b.m21) + (a.m03 * b.m31);
        result.m02 = (a.m00 * b.m02) + (a.m01 * b.m12) + (a.m02 * b.m22) + (a.m03 * b.m32);
        result.m03 = (a.m00 * b.m03) + (a.m01 * b.m13) + (a.m02 * b.m23) + (a.m03 * b.m33);

        result.m10 = (a.m10 * b.m00) + (a.m11 * b.m10) + (a.m12 * b.m20) + (a.m13 * b.m30);
        result.m11 = (a.m10 * b.m01) + (a.m11 * b.m11) + (a.m12 * b.m21) + (a.m13 * b.m31);
        result.m12 = (a.m10 * b.m02) + (a.m11 * b.m12) + (a.m12 * b.m22) + (a.m13 * b.m32);
        result.m13 = (a.m10 * b.m03) + (a.m11 * b.m13) + (a.m12 * b.m23) + (a.m13 * b.m33);

        result.m20 = (a.m20 * b.m00) + (a.m21 * b.m10) + (a.m22 * b.m20) + (a.m23 * b.m30);
        result.m21 = (a.m20 * b.m01) + (a.m21 * b.m11) + (a.m22 * b.m21) + (a.m23 * b.m31);
        result.m22 = (a.m20 * b.m02) + (a.m21 * b.m12) + (a.m22 * b.m22) + (a.m23 * b.m32);
        result.m23 = (a.m20 * b.m03) + (a.m21 * b.m13) + (a.m22 * b.m23) + (a.m23 * b.m33);

        result.m30 = (a.m30 * b.m00) + (a.m31 * b.m10) + (a.m32 * b.m20) + (a.m33 * b.m30);
        result.m31 = (a.m30 * b.m01) + (a.m31 * b.m11) + (a.m32 * b.m21) + (a.m33 * b.m31);
        result.m32 = (a.m30 * b.m02) + (a.m31 * b.m12) + (a.m32 * b.m22) + (a.m33 * b.m32);
        result.m33 = (a.m30 * b.m03) + (a.m31 * b.m13) + (a.m32 * b.m23) + (a.m33 * b.m33);

        return result;
    }

    inline Vector4 operator * (const Vector4& a, const Matrix4& b)
    {
        Vector4 result;

        result.x = (a.x * b.m00) + (a.y * b.m10) + (a.z * b.m20) + (a.z * b.m30);
        result.y = (a.x * b.m01) + (a.y * b.m11) + (a.z * b.m21) + (a.z * b.m31);
        result.z = (a.x * b.m02) + (a.y * b.m12) + (a.z * b.m22) + (a.z * b.m32);
        result.w = (a.x * b.m03) + (a.y * b.m13) + (a.z * b.m23) + (a.z * b.m33);

        return result;
    }
}

#endif
