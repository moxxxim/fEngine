#ifndef Matrix4_hpp
#define Matrix4_hpp

#include <Feng/Math/Vector4.h>
#include <cstdint>
#include <limits>
#include <utility>

namespace Feng
{
    class Matrix4 final
    {
    public:
        Matrix4() = default;
        constexpr Matrix4(
            float a00, float a01, float a02, float a03,
            float a10, float a11, float a12, float a13,
            float a20, float a21, float a22, float a23,
            float a30, float a31, float a32, float a33)
            : m { a00, a01, a02, a03
            , a10, a11, a12, a13
            , a20, a21, a22, a23
            , a30, a31, a32, a33 }
        {}
        Matrix4(const Matrix4&) = default;
        Matrix4(Matrix4&&) = default;

        bool HasInverse() const;
        float Determinant() const;
        float Trace() const;
        bool TryInvert(Matrix4& inverted) const;
        Matrix4 ToTransposed() const;

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

        struct Component
        {
            float _00;
            float _01;
            float _02;
            float _03;

            float _10;
            float _11;
            float _12;
            float _13;

            float _20;
            float _21;
            float _22;
            float _23;

            float _30;
            float _31;
            float _32;
            float _33;
        };
        
        union
        {
            float data[16];
            float mat[4][4];
            Vector4 rows[4];
            Component m;
        };
    };
}

namespace Feng
{
    inline bool Matrix4::HasInverse() const
    {
        constexpr float epsilon = std::numeric_limits<float>::epsilon();
        float determinant = Determinant();
        return !((epsilon < determinant) && (determinant < epsilon));
    }

    inline float Matrix4::Determinant() const
    {
        return (m._00 * m._11 - m._10 * m._01) * (m._22 * m._33 - m._32 * m._23)
        - (m._00 * m._21 - m._20 * m._01) * (m._12 * m._33 - m._32 * m._13)
        + (m._00 * m._31 - m._30 * m._01) * (m._12 * m._23 - m._22 * m._13)
        + (m._10 * m._21 - m._20 * m._11) * (m._02 * m._33 - m._32 * m._03)
        - (m._10 * m._31 - m._30 * m._11) * (m._02 * m._23 - m._22 * m._03)
        + (m._20 * m._31 - m._30 * m._21) * (m._02 * m._13 - m._12 * m._03);
    }

    inline float Matrix4::Trace() const
    {
        return m._00 + m._11 + m._22 + m._33;
    }

    inline bool Matrix4::TryInvert(Matrix4& inv) const
    {
        constexpr float epsilon = std::numeric_limits<float>::epsilon();
        float d = Determinant();
        if((epsilon < d) && (d < epsilon))
        {
            return false;
        }

        d = 1.f / d;

        const Matrix4& meCopy = *this;

        inv.m._00 = d * (meCopy.m._11 * (meCopy.m._22 * meCopy.m._33 - meCopy.m._32 * meCopy.m._23) + meCopy.m._21 * (meCopy.m._32 * meCopy.m._13 - meCopy.m._12 * meCopy.m._33) + meCopy.m._31 * (meCopy.m._12 * meCopy.m._23 - meCopy.m._22 * meCopy.m._13));
        inv.m._10 = d * (meCopy.m._12 * (meCopy.m._20 * meCopy.m._33 - meCopy.m._30 * meCopy.m._23) + meCopy.m._22 * (meCopy.m._30 * meCopy.m._13 - meCopy.m._10 * meCopy.m._33) + meCopy.m._32 * (meCopy.m._10 * meCopy.m._23 - meCopy.m._20 * meCopy.m._13));
        inv.m._20 = d * (meCopy.m._13 * (meCopy.m._20 * meCopy.m._31 - meCopy.m._30 * meCopy.m._21) + meCopy.m._23 * (meCopy.m._30 * meCopy.m._11 - meCopy.m._10 * meCopy.m._31) + meCopy.m._33 * (meCopy.m._10 * meCopy.m._21 - meCopy.m._20 * meCopy.m._11));
        inv.m._30 = d * (meCopy.m._10 * (meCopy.m._31 * meCopy.m._22 - meCopy.m._21 * meCopy.m._32) + meCopy.m._20 * (meCopy.m._11 * meCopy.m._32 - meCopy.m._31 * meCopy.m._12) + meCopy.m._30 * (meCopy.m._21 * meCopy.m._12 - meCopy.m._11 * meCopy.m._22));

        inv.m._01 = d * (meCopy.m._21 * (meCopy.m._02 * meCopy.m._33 - meCopy.m._32 * meCopy.m._03) + meCopy.m._31 * (meCopy.m._22 * meCopy.m._03 - meCopy.m._02 * meCopy.m._23) + meCopy.m._01 * (meCopy.m._32 * meCopy.m._23 - meCopy.m._22 * meCopy.m._33));
        inv.m._11 = d * (meCopy.m._22 * (meCopy.m._00 * meCopy.m._33 - meCopy.m._30 * meCopy.m._03) + meCopy.m._32 * (meCopy.m._20 * meCopy.m._03 - meCopy.m._00 * meCopy.m._23) + meCopy.m._02 * (meCopy.m._30 * meCopy.m._23 - meCopy.m._20 * meCopy.m._33));
        inv.m._21 = d * (meCopy.m._23 * (meCopy.m._00 * meCopy.m._31 - meCopy.m._30 * meCopy.m._01) + meCopy.m._33 * (meCopy.m._20 * meCopy.m._01 - meCopy.m._00 * meCopy.m._21) + meCopy.m._03 * (meCopy.m._30 * meCopy.m._21 - meCopy.m._20 * meCopy.m._31));
        inv.m._31 = d * (meCopy.m._20 * (meCopy.m._31 * meCopy.m._02 - meCopy.m._01 * meCopy.m._32) + meCopy.m._30 * (meCopy.m._01 * meCopy.m._22 - meCopy.m._21 * meCopy.m._02) + meCopy.m._00 * (meCopy.m._21 * meCopy.m._32 - meCopy.m._31 * meCopy.m._22));

        inv.m._02 = d * (meCopy.m._31 * (meCopy.m._02 * meCopy.m._13 - meCopy.m._12 * meCopy.m._03) + meCopy.m._01 * (meCopy.m._12 * meCopy.m._33 - meCopy.m._32 * meCopy.m._13) + meCopy.m._11 * (meCopy.m._32 * meCopy.m._03 - meCopy.m._02 * meCopy.m._33));
        inv.m._12 = d * (meCopy.m._32 * (meCopy.m._00 * meCopy.m._13 - meCopy.m._10 * meCopy.m._03) + meCopy.m._02 * (meCopy.m._10 * meCopy.m._33 - meCopy.m._30 * meCopy.m._13) + meCopy.m._12 * (meCopy.m._30 * meCopy.m._03 - meCopy.m._00 * meCopy.m._33));
        inv.m._22 = d * (meCopy.m._33 * (meCopy.m._00 * meCopy.m._11 - meCopy.m._10 * meCopy.m._01) + meCopy.m._03 * (meCopy.m._10 * meCopy.m._31 - meCopy.m._30 * meCopy.m._11) + meCopy.m._13 * (meCopy.m._30 * meCopy.m._01 - meCopy.m._00 * meCopy.m._31));
        inv.m._32 = d * (meCopy.m._30 * (meCopy.m._11 * meCopy.m._02 - meCopy.m._01 * meCopy.m._12) + meCopy.m._00 * (meCopy.m._31 * meCopy.m._12 - meCopy.m._11 * meCopy.m._32) + meCopy.m._10 * (meCopy.m._01 * meCopy.m._32 - meCopy.m._31 * meCopy.m._02));

        inv.m._03 = d * (meCopy.m._01 * (meCopy.m._22 * meCopy.m._13 - meCopy.m._12 * meCopy.m._23) + meCopy.m._11 * (meCopy.m._02 * meCopy.m._23 - meCopy.m._22 * meCopy.m._03) + meCopy.m._21 * (meCopy.m._12 * meCopy.m._03 - meCopy.m._02 * meCopy.m._13));
        inv.m._13 = d * (meCopy.m._02 * (meCopy.m._20 * meCopy.m._13 - meCopy.m._10 * meCopy.m._23) + meCopy.m._12 * (meCopy.m._00 * meCopy.m._23 - meCopy.m._20 * meCopy.m._03) + meCopy.m._22 * (meCopy.m._10 * meCopy.m._03 - meCopy.m._00 * meCopy.m._13));
        inv.m._23 = d * (meCopy.m._03 * (meCopy.m._20 * meCopy.m._11 - meCopy.m._10 * meCopy.m._21) + meCopy.m._13 * (meCopy.m._00 * meCopy.m._21 - meCopy.m._20 * meCopy.m._01) + meCopy.m._23 * (meCopy.m._10 * meCopy.m._01 - meCopy.m._00 * meCopy.m._11));
        inv.m._33 = d * (meCopy.m._00 * (meCopy.m._11 * meCopy.m._22 - meCopy.m._21 * meCopy.m._12) + meCopy.m._10 * (meCopy.m._21 * meCopy.m._02 - meCopy.m._01 * meCopy.m._22) + meCopy.m._20 * (meCopy.m._01 * meCopy.m._12 - meCopy.m._11 * meCopy.m._02));

        return true;
    }

    inline Matrix4 Matrix4::ToTransposed() const
    {
        Matrix4 transposed = *this;
        transposed.Transpose();

        return transposed;
    }

    inline void Matrix4::Transpose()
    {
        std::swap(m._01, m._10);
        std::swap(m._02, m._20);
        std::swap(m._03, m._30);
        std::swap(m._12, m._21);
        std::swap(m._13, m._31);
        std::swap(m._23, m._32);
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

        result.m._00 = (a.m._00 * b.m._00) + (a.m._01 * b.m._10) + (a.m._02 * b.m._20) + (a.m._03 * b.m._30);
        result.m._01 = (a.m._00 * b.m._01) + (a.m._01 * b.m._11) + (a.m._02 * b.m._21) + (a.m._03 * b.m._31);
        result.m._02 = (a.m._00 * b.m._02) + (a.m._01 * b.m._12) + (a.m._02 * b.m._22) + (a.m._03 * b.m._32);
        result.m._03 = (a.m._00 * b.m._03) + (a.m._01 * b.m._13) + (a.m._02 * b.m._23) + (a.m._03 * b.m._33);
        result.m._10 = (a.m._10 * b.m._00) + (a.m._11 * b.m._10) + (a.m._12 * b.m._20) + (a.m._13 * b.m._30);
        result.m._11 = (a.m._10 * b.m._01) + (a.m._11 * b.m._11) + (a.m._12 * b.m._21) + (a.m._13 * b.m._31);
        result.m._12 = (a.m._10 * b.m._02) + (a.m._11 * b.m._12) + (a.m._12 * b.m._22) + (a.m._13 * b.m._32);
        result.m._13 = (a.m._10 * b.m._03) + (a.m._11 * b.m._13) + (a.m._12 * b.m._23) + (a.m._13 * b.m._33);
        result.m._20 = (a.m._20 * b.m._00) + (a.m._21 * b.m._10) + (a.m._22 * b.m._20) + (a.m._23 * b.m._30);
        result.m._21 = (a.m._20 * b.m._01) + (a.m._21 * b.m._11) + (a.m._22 * b.m._21) + (a.m._23 * b.m._31);
        result.m._22 = (a.m._20 * b.m._02) + (a.m._21 * b.m._12) + (a.m._22 * b.m._22) + (a.m._23 * b.m._32);
        result.m._23 = (a.m._20 * b.m._03) + (a.m._21 * b.m._13) + (a.m._22 * b.m._23) + (a.m._23 * b.m._33);
        result.m._30 = (a.m._30 * b.m._00) + (a.m._31 * b.m._10) + (a.m._32 * b.m._20) + (a.m._33 * b.m._30);
        result.m._31 = (a.m._30 * b.m._01) + (a.m._31 * b.m._11) + (a.m._32 * b.m._21) + (a.m._33 * b.m._31);
        result.m._32 = (a.m._30 * b.m._02) + (a.m._31 * b.m._12) + (a.m._32 * b.m._22) + (a.m._33 * b.m._32);
        result.m._33 = (a.m._30 * b.m._03) + (a.m._31 * b.m._13) + (a.m._32 * b.m._23) + (a.m._33 * b.m._33);

        return result;
    }

    inline Vector4 operator * (const Vector4& a, const Matrix4& b)
    {
        Vector4 result;

        result.coord.x = (a.coord.x * b.m._00) + (a.coord.y * b.m._10) + (a.coord.z * b.m._20) + (a.coord.w * b.m._30);
        result.coord.y = (a.coord.x * b.m._01) + (a.coord.y * b.m._11) + (a.coord.z * b.m._21) + (a.coord.w * b.m._31);
        result.coord.z = (a.coord.x * b.m._02) + (a.coord.y * b.m._12) + (a.coord.z * b.m._22) + (a.coord.w * b.m._32);
        result.coord.w = (a.coord.x * b.m._03) + (a.coord.y * b.m._13) + (a.coord.z * b.m._23) + (a.coord.w * b.m._33);

        return result;
    }
}

#endif
