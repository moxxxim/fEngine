#ifndef Matrix4_hpp
#define Matrix4_hpp

#include <FEngine/Math/Vector4.h>
#include <cstdint>

namespace fengine
{
    class Matrix4 final
    {
    public:
        Matrix4() = default;
        Matrix4(const Matrix4&) = default;
        Matrix4(Matrix4&&) = default;

        bool HasInverse() const;
        float Determinant() const;
        float Trace() const;
        bool TryInvert(Matrix4& inverted) const;
        Matrix4 Transposed() const;

        void Transpose();

        Matrix4& operator = (const Matrix4& other) = default;
        Matrix4& operator = (Matrix4&& other) = default;
        Matrix4& operator *= (const Matrix4& other);
        float operator () (uint8_t i, uint8_t j) const;
        float& operator () (uint8_t i, uint8_t j);

        friend Matrix4 operator * (const Matrix4& a, const Matrix4& b);
        friend Vector4 operator * (const Vector4& a, const Matrix4& b);

        static const Matrix4 Zero;
        static const Matrix4 Identity;

        union
        {
            float data[16];
            float mat[4][4];
            Vector4 rows[4];
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
    inline bool Matrix4::HasInverse() const
    {
        float determinant = Determinant();
        float epsilon = std::numeric_limits<float>::epsilon();
        return !((epsilon < determinant) && (determinant < epsilon));
    }

    inline float Matrix4::Determinant() const
    {
        return (m00 * m11 - m10 * m01) * (m22 * m33 - m32 * m23)
        - (m00 * m21 - m20 * m01) * (m12 * m33 - m32 * m13)
        + (m00 * m31 - m30 * m01) * (m12 * m23 - m22 * m13)
        + (m10 * m21 - m20 * m11) * (m02 * m33 - m32 * m03)
        - (m10 * m31 - m30 * m11) * (m02 * m23 - m22 * m03)
        + (m20 * m31 - m30 * m21) * (m02 * m13 - m12 * m03);
    }

    inline float Matrix4::Trace() const
    {
        return m00 + m11 + m22 + m33;
    }

    inline bool Matrix4::TryInvert(Matrix4& inv) const
    {
        const float epsilon = std::numeric_limits<float>::epsilon();
        float d = Determinant();
        if((epsilon < d) && (d < epsilon))
        {
            return false;
        }

        d = 1.f / d;

        const Matrix4& m = *this;

        inv.m00 = d * (m11 * (m22 * m33 - m32 * m23) + m21 * (m32 * m13 - m12 * m33) + m31 * (m12 * m23 - m22 * m13));
        inv.m10 = d * (m12 * (m20 * m33 - m30 * m23) + m22 * (m30 * m13 - m10 * m33) + m32 * (m10 * m23 - m20 * m13));
        inv.m20 = d * (m13 * (m20 * m31 - m30 * m21) + m23 * (m30 * m11 - m10 * m31) + m33 * (m10 * m21 - m20 * m11));
        inv.m30 = d * (m10 * (m31 * m22 - m21 * m32) + m20 * (m11 * m32 - m31 * m12) + m30 * (m21 * m12 - m11 * m22));

        inv.m01 = d * (m21 * (m02 * m33 - m32 * m03) + m31 * (m22 * m03 - m02 * m23) + m01 * (m32 * m23 - m22 * m33));
        inv.m11 = d * (m22 * (m00 * m33 - m30 * m03) + m32 * (m20 * m03 - m00 * m23) + m02 * (m30 * m23 - m20 * m33));
        inv.m21 = d * (m23 * (m00 * m31 - m30 * m01) + m33 * (m20 * m01 - m00 * m21) + m03 * (m30 * m21 - m20 * m31));
        inv.m31 = d * (m20 * (m31 * m02 - m01 * m32) + m30 * (m01 * m22 - m21 * m02) + m00 * (m21 * m32 - m31 * m22));

        inv.m02 = d * (m31 * (m02 * m13 - m12 * m03) + m01 * (m12 * m33 - m32 * m13) + m11 * (m32 * m03 - m02 * m33));
        inv.m12 = d * (m32 * (m00 * m13 - m10 * m03) + m02 * (m10 * m33 - m30 * m13) + m12 * (m30 * m03 - m00 * m33));
        inv.m22 = d * (m33 * (m00 * m11 - m10 * m01) + m03 * (m10 * m31 - m30 * m11) + m13 * (m30 * m01 - m00 * m31));
        inv.m32 = d * (m30 * (m11 * m02 - m01 * m12) + m00 * (m31 * m12 - m11 * m32) + m10 * (m01 * m32 - m31 * m02));

        inv.m03 = d * (m01 * (m22 * m13 - m12 * m23) + m11 * (m02 * m23 - m22 * m03) + m21 * (m12 * m03 - m02 * m13));
        inv.m13 = d * (m02 * (m20 * m13 - m10 * m23) + m12 * (m00 * m23 - m20 * m03) + m22 * (m10 * m03 - m00 * m13));
        inv.m23 = d * (m03 * (m20 * m11 - m10 * m21) + m13 * (m00 * m21 - m20 * m01) + m23 * (m10 * m01 - m00 * m11));
        inv.m33 = d * (m00 * (m11 * m22 - m21 * m12) + m10 * (m21 * m02 - m01 * m22) + m20 * (m01 * m12 - m11 * m02));

        return true;
    }

    inline Matrix4 Matrix4::Transposed() const
    {
        Matrix4 transposed = *this;
        transposed.Transpose();

        return transposed;
    }

    inline void Matrix4::Transpose()
    {
        std::swap(m01, m10);
        std::swap(m02, m20);
        std::swap(m03, m30);
        std::swap(m12, m21);
        std::swap(m13, m31);
        std::swap(m23, m32);
    }

    inline Matrix4& Matrix4::operator *= (const Matrix4& other)
    {
        *this = *this * other;
        return *this;
    }

    inline float Matrix4::operator () (uint8_t i, uint8_t j) const
    {
        return mat[i][j];
    }

    inline float& Matrix4::operator () (uint8_t i, uint8_t j)
    {
        return mat[i][j];
    }

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
