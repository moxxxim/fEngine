#ifndef Matrix3_hpp
#define Matrix3_hpp

#include <Feng/Math/Vector3.h>
#include <cstdint>
#include <limits>
#include <utility>

namespace Feng
{
    class Matrix3 final
    {
    public:
        static constexpr const Matrix3 Zero()
        {
            return Matrix3 {0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f,
                            0.f, 0.f, 0.f };
        }
        
        static constexpr const Matrix3 Identity()
        {
            return Matrix3 {1.f, 0.f, 0.f,
                            0.f, 1.f, 0.f,
                            0.f, 0.f, 1.f };
        }
        
        static constexpr Matrix3 Rows(const Vector3& ex, const Vector3& ey, const Vector3& ez)
        {
            return Matrix3 {ex.coord.x, ex.coord.y, ex.coord.z,
                            ey.coord.x, ey.coord.y, ey.coord.z,
                            ez.coord.x, ez.coord.y, ez.coord.z };
        }
        
        static constexpr Matrix3 Diag(float a00, float a11, float a22)
        {
            return Matrix3 {a00, 0.f, 0.f,
                            0.f, a11, 0.f,
                            0.f, 0.f, a22 };
        }
        
        static constexpr Matrix3 Diag(const Vector3& diag)
        {
            return Matrix3::Diag(diag.coord.x, diag.coord.y, diag.coord.z);
        }
        
        static constexpr Matrix3 Diag(float value)
        {
            return Matrix3::Diag(value, value, value);
        }

        Matrix3() = default;
        constexpr Matrix3(
            float a00, float a01, float a02,
            float a10, float a11, float a12,
            float a20, float a21, float a22)
            : m{ a00, a01, a02
            , a10, a11, a12
            , a20, a21, a22 }
        {}
        constexpr Matrix3(const Matrix3&) = default;
        constexpr Matrix3(Matrix3&&) = default;

        bool HasInverse() const;
        constexpr float Determinant() const
        {
            return m._00 * (m._11 * m._22 - m._21 * m._12) - m._01 * (m._10 * m._22 - m._20 * m._12) + m._02 * (m._10 * m._21 - m._20 * m._11);
        }
        
        constexpr float Trace() const
        {
            return m._00 + m._11 + m._22;
        }

        bool TryInvert(Matrix3& inverted) const;
        
        constexpr Matrix3 Transposed() const
        {
            return Matrix3 {
                m._00, m._10, m._20,
                m._01, m._11, m._21,
                m._02, m._12, m._22
            };
        }

        void Transpose();

        constexpr Matrix3& operator = (const Matrix3& other) = default;
        constexpr Matrix3& operator = (Matrix3&& other) = default;
        Matrix3& operator *= (const Matrix3& other);
        float operator()(uint8_t i, uint8_t j) const;
        float& operator()(uint8_t i, uint8_t j);

        friend Matrix3 operator * (const Matrix3& a, const Matrix3& b);
        friend Vector3 operator * (const Vector3& a, const Matrix3& b);
        friend Matrix3 operator * (float a, const Matrix3& m);
        friend Matrix3 operator * (const Matrix3& m, float a);
        friend Matrix3 operator / (const Matrix3& m, float a);

        union
        {
            float data[9];
            float mat[3][3];
            Vector3 rows[3];
            struct Component
            {
                float _00;
                float _01;
                float _02;
                float _10;
                float _11;
                float _12;
                float _20;
                float _21;
                float _22;
            } m;
        };
    };
}

namespace Feng
{
    inline bool Matrix3::HasInverse() const
    {
        constexpr float epsilon = std::numeric_limits<float>::epsilon();
        float determinant = Determinant();
        return !((epsilon < determinant) && (determinant < epsilon));
    }

    inline bool Matrix3::TryInvert(Matrix3& inverted) const
    {
        throw "Not Implemented";

        inverted = Matrix3::Identity();

        constexpr float epsilon = std::numeric_limits<float>::epsilon();
        float det = Determinant();
        if((epsilon < det) && (det < epsilon))
        {
            return false;
        }

        return true;
    }

    inline void Matrix3::Transpose()
    {
        std::swap(m._01, m._10);
        std::swap(m._02, m._20);
        std::swap(m._12, m._21);
    }

    inline Matrix3& Matrix3::operator *= (const Matrix3& other)
    {
        *this = *this * other;
        return *this;
    }

    inline float Matrix3::operator()(uint8_t i, uint8_t j) const
    {
        return mat[i][j];
    }

    inline float& Matrix3::operator()(uint8_t i, uint8_t j)
    {
        return mat[i][j];
    }

    inline Matrix3 operator * (const Matrix3& a, const Matrix3& b)
    {
        Matrix3 result;

        result.m._00 = (a.m._00 * b.m._00) + (a.m._01 * b.m._10) + (a.m._02 * b.m._20);
        result.m._01 = (a.m._00 * b.m._01) + (a.m._01 * b.m._11) + (a.m._02 * b.m._21);
        result.m._02 = (a.m._00 * b.m._02) + (a.m._01 * b.m._12) + (a.m._02 * b.m._22);
        result.m._10 = (a.m._10 * b.m._00) + (a.m._11 * b.m._10) + (a.m._12 * b.m._20);
        result.m._11 = (a.m._10 * b.m._01) + (a.m._11 * b.m._11) + (a.m._12 * b.m._21);
        result.m._12 = (a.m._10 * b.m._02) + (a.m._11 * b.m._12) + (a.m._12 * b.m._22);
        result.m._20 = (a.m._20 * b.m._00) + (a.m._21 * b.m._10) + (a.m._22 * b.m._20);
        result.m._21 = (a.m._20 * b.m._01) + (a.m._21 * b.m._11) + (a.m._22 * b.m._21);
        result.m._22 = (a.m._20 * b.m._02) + (a.m._21 * b.m._12) + (a.m._22 * b.m._22);

        return result;

    }

    inline Vector3 operator * (const Vector3& a, const Matrix3& b)
    {
        Vector3 result;

        result.coord.x = (a.coord.x * b.m._00) + (a.coord.y * b.m._10) + (a.coord.z * b.m._20);
        result.coord.y = (a.coord.x * b.m._01) + (a.coord.y * b.m._11) + (a.coord.z * b.m._21);
        result.coord.z = (a.coord.x * b.m._02) + (a.coord.y * b.m._12) + (a.coord.z * b.m._22);

        return result;
    }

    inline Matrix3 operator * (float a, const Matrix3& m)
    {
        Matrix3 result = m;
        for(int i = 0; i < 9; ++i)
        {
            result.data[i] *= a;
        }

        return result;
    }

    inline Matrix3 operator * (const Matrix3& m, float a)
    {
        return a * m;
    }

    inline Matrix3 operator / (const Matrix3& m, float a)
    {
        return (1 / a) * m;
    }
}

#endif
