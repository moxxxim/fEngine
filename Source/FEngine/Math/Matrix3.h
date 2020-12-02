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

        bool HasInverse() const;
        float Determinant() const;
        float Trace() const;
        bool TryInvert(Matrix3& inverted) const;
        Matrix3 Transposed() const;

        void Transpose();

        Matrix3& operator = (const Matrix3& other) = default;
        Matrix3& operator = (Matrix3&& other) = default;

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

namespace fengine
{
    inline bool Matrix3::HasInverse() const
    {
        float determinant = Determinant();
        float epsilon = std::numeric_limits<float>::epsilon();
        return !((epsilon < determinant) && (determinant < epsilon));
    }

    inline float Matrix3::Determinant() const
    {
        return m00 * (m11 * m22 - m21 * m12) - m01 * (m10 * m22 - m20 * m12) + m02 * (m10 * m21 - m20 * m11);
    }

    inline float Matrix3::Trace() const
    {
        return m00 + m11 + m22;
    }

    inline bool Matrix3::TryInvert(Matrix3& inverted) const
    {
        float det = Determinant();
        float epsilon = std::numeric_limits<float>::epsilon();
        if((epsilon < det) && (det < epsilon))
        {
            return false;
        }

        return true;
    }

    inline Matrix3 Matrix3::Transposed() const
    {
        Matrix3 inverted = *this;
        inverted.Transpose();

        return inverted;
    }

    inline void Matrix3::Transpose()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m12, m21);
    }
}

#endif
