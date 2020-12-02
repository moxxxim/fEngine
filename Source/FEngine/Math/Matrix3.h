#ifndef Matrix3_hpp
#define Matrix3_hpp

#include <FEngine/Math/Vector3.h>

namespace fengine
{
    class Matrix3 final
    {
    public:
        static Matrix3 FromBasis(const Vector3& ex, const Vector3& ey, const Vector3& ez);
        static Matrix3 Diag(const Vector3& diag);
        static Matrix3 Diag(float value);

        Matrix3() = default;
        Matrix3(const Matrix3&) = default;
        Matrix3(Matrix3&&) = default;

        friend Matrix3 operator * (const Matrix3& a, const Matrix3& b);
        friend Matrix3 operator * (const Vector3& a, const Matrix3& b);
        friend Matrix3 operator * (float a, const Matrix3& m);
        friend Matrix3 operator * (const Matrix3& m, float a);
        friend Matrix3 operator / (const Matrix3& m, float a);

        static const Matrix3 Zero;
        static const Matrix3 Identity;

        union
        {
            float data[9];
            struct
            {
                float m00;
                float m01;
                float m02;
                float m10;
                float m11;
                float m12;
                float m20;
                float m21;
                float m22;
            };
        };
    };
}
#endif
