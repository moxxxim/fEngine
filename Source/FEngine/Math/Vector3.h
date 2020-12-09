#pragma once

#include <cmath>
#include <cstring>

namespace feng
{
    class Vector3 final
    {
    public:
        constexpr static float Dot(const Vector3& a, const Vector3& b)
        {
            return (a.x * b.x) + (a.y * b.y) + (a.z * b.z);
        }

        constexpr static Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            float x = (a.y * b.z) - (a.z * b.y);
            float y = (a.z * b.x) - (a.x * b.z);
            float z = (a.x * b.y) - (a.y * b.x);

            return Vector3 {x, y, z};
        }

        constexpr static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
        {
            return ((1 - t) * a) + (t * b);
        }

        static float Distance(const Vector3& a, const Vector3& b)
        {
            return (b - a).Length();
        }

        Vector3() = default;
        constexpr Vector3(float aX, float aY, float aZ)
        : x {aX} , y {aY} , z {aZ}
        { }
        constexpr Vector3(const Vector3&) = default;
        constexpr Vector3(Vector3&&) = default;

        float Length() const;
        Vector3 ToNormalized() const;
        void Normalize();

        constexpr float LengthSqr() const
        {
            return Vector3::Dot(*this, *this);
        }

        constexpr Vector3 ProjectOn(const Vector3& on) const
        {
            return on * Vector3::Dot(*this, on);
        }

        constexpr Vector3& operator = (const Vector3& other) = default;
        constexpr Vector3& operator = (Vector3&& other) = default;
        constexpr Vector3 operator - () const
        {
            return Vector3{-x, -y, -z};
        }

        Vector3& operator += (const Vector3& other);
        Vector3& operator -= (const Vector3& other);
        Vector3& operator *= (float value);
        friend bool operator == (const Vector3& a, const Vector3& b);
        friend bool operator != (const Vector3& a, const Vector3& b);
        constexpr friend Vector3 operator * (const Vector3& v, float a)
        {
            return Vector3 {v.x * a, v.y * a, v.z * a};
        }

        constexpr friend Vector3 operator * (float a, const Vector3& v)
        {
            return v * a;
        }

        constexpr friend Vector3 operator / (const Vector3& v, float a)
        {
            return Vector3 {v.x / a, v.y / a, v.z / a};
        }

        constexpr friend Vector3 operator + (const Vector3& a, const Vector3& b)
        {
            return Vector3 {a.x + b.x, a.y + b.y, a.z + b.z};
        }

        constexpr friend Vector3 operator - (const Vector3& a, const Vector3& b)
        {
            return a + (-b);
        }

        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 OneX;
        static const Vector3 OneY;
        static const Vector3 OneZ;

        union
        {
            float data[3];
            struct
            {
                float x;
                float y;
                float z;
            };
        };
    };
}

namespace feng
{
    inline float Vector3::Length() const
    {
        return std::sqrt(Vector3::LengthSqr());
    }

    inline Vector3 Vector3::ToNormalized() const
    {
        Vector3 normalized = *this;
        normalized.Normalize();

        return normalized;
    }

    inline void Vector3::Normalize()
    {
        float norm = Length();
        x /= norm;
        y /= norm;
        z /= norm;
    }

    inline bool operator == (const Vector3& a, const Vector3& b)
    {
        return (std::memcmp(a.data, b.data, sizeof(Vector3)) == 0);
    }

    inline bool operator != (const Vector3& a, const Vector3& b)
    {
        return !(a == b);
    }

    inline Vector3& Vector3::operator += (const Vector3& other)
    {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    inline Vector3& Vector3::operator -= (const Vector3& other)
    {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    inline Vector3& Vector3::operator *= (float value)
    {
        x *= value;
        y *= value;
        z *= value;

        return *this;
    }
}
