#ifndef Matrix3_hpp
#define Matrix3_hpp

#include <FEngine/Math/Vector3.h>
#include <cstdint>

namespace feng
{
    class Matrix3 final
    {
    public:
        static Matrix3 Rows(const Vector3& ex, const Vector3& ey, const Vector3& ez);
        static Matrix3 Diag(const Vector3& diag);
        static Matrix3 Diag(float a00, float a11, float a22);
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
        Matrix3& operator *= (const Matrix3& other);
        float operator()(uint8_t i, uint8_t j) const;
        float& operator()(uint8_t i, uint8_t j);

        friend Matrix3 operator * (const Matrix3& a, const Matrix3& b);
        friend Vector3 operator * (const Vector3& a, const Matrix3& b);
        friend Matrix3 operator * (float a, const Matrix3& m);
        friend Matrix3 operator * (const Matrix3& m, float a);
        friend Matrix3 operator / (const Matrix3& m, float a);

        static const Matrix3 Zero;
        static const Matrix3 Identity;

        union
        {
            float data[9];
            float mat[3][3];
            Vector3 rows[3];
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

namespace feng
{
    inline Matrix3 Matrix3::Rows(const Vector3& ex, const Vector3& ey, const Vector3& ez)
    {
        Matrix3 matrix;

        matrix.m00 = ex.x;
        matrix.m01 = ex.y;
        matrix.m02 = ex.z;

        matrix.m10 = ey.x;
        matrix.m11 = ey.y;
        matrix.m12 = ey.z;

        matrix.m20 = ez.x;
        matrix.m21 = ez.y;
        matrix.m22 = ez.z;

        return matrix;
    }

    inline Matrix3 Matrix3::Diag(const Vector3& diag)
    {
        return Matrix3::Diag(diag.x, diag.y, diag.z);
    }

    inline Matrix3 Matrix3::Diag(float a00, float a11, float a22)
    {
        Matrix3 matrix = Matrix3::Zero;

        matrix.m00 = a00;
        matrix.m11 = a11;
        matrix.m22 = a22;

        return matrix;
    }

    inline Matrix3 Matrix3::Diag(float value)
    {
        return Matrix3::Diag(value, value, value);
    }

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

    inline Matrix3& Matrix3::operator *= (const Matrix3& other)
    {
        *this = *this * other;
        return *this;
    }

    inline float Matrix3::operator()(uint8_t i, uint8_t j) const
    {
        return mat[j][j];
    }

    inline float& Matrix3::operator()(uint8_t i, uint8_t j)
    {
        return mat[j][j];
    }

    inline Matrix3 operator * (const Matrix3& a, const Matrix3& b)
    {
        Matrix3 result;

        result.m00 = (a.m00 * b.m00) + (a.m01 * b.m10) + (a.m02 * b.m20);
        result.m01 = (a.m00 * b.m01) + (a.m01 * b.m11) + (a.m02 * b.m21);
        result.m02 = (a.m00 * b.m02) + (a.m01 * b.m12) + (a.m02 * b.m22);

        result.m10 = (a.m10 * b.m00) + (a.m11 * b.m10) + (a.m12 * b.m20);
        result.m11 = (a.m10 * b.m01) + (a.m11 * b.m11) + (a.m12 * b.m21);
        result.m12 = (a.m10 * b.m02) + (a.m11 * b.m12) + (a.m12 * b.m22);

        result.m20 = (a.m20 * b.m00) + (a.m21 * b.m10) + (a.m22 * b.m20);
        result.m21 = (a.m20 * b.m01) + (a.m21 * b.m11) + (a.m22 * b.m21);
        result.m22 = (a.m20 * b.m02) + (a.m21 * b.m12) + (a.m22 * b.m22);

        return result;

    }

    inline Vector3 operator * (const Vector3& a, const Matrix3& b)
    {
        Vector3 result;

        result.x = (a.x * b.m00) + (a.y * b.m10) + (a.z * b.m20);
        result.y = (a.x * b.m01) + (a.y * b.m11) + (a.z * b.m21);
        result.z = (a.x * b.m02) + (a.y * b.m12) + (a.z * b.m22);

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
