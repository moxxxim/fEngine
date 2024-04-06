#pragma once

#include <cmath>
#include <cstring>

namespace Feng
{
    class Vector3 final
    {
    public:
        constexpr static float Dot(const Vector3& a, const Vector3& b)
        {
            return (a.coord.x * b.coord.x) + (a.coord.y * b.coord.y) + (a.coord.z * b.coord.z);
        }

        constexpr static Vector3 Cross(const Vector3& a, const Vector3& b)
        {
            const float x = (a.coord.y * b.coord.z) - (a.coord.z * b.coord.y);
            const float y = (a.coord.z * b.coord.x) - (a.coord.x * b.coord.z);
            const float z = (a.coord.x * b.coord.y) - (a.coord.y * b.coord.x);

            return Vector3 {x, y, z};
        }

        constexpr static Vector3 Lerp(const Vector3& a, const Vector3& b, float t)
        {
            return ((1 - t) * a) + (t * b);
        }

        constexpr static float DistanceSqr(const Vector3& a, const Vector3& b)
        {
            return (b - a).LengthSqr();
        }

        static float Distance(const Vector3& a, const Vector3& b)
        {
            return (b - a).Length();
        }

        Vector3() = default;
        constexpr Vector3(float aX, float aY, float aZ)
        : coord {aX, aY, aZ}
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

        constexpr Vector3 Reflect(const Vector3& norm) const
        {
            return *this - 2 * (Vector3::Dot(*this, norm)) * norm;
        }

        constexpr Vector3& operator = (const Vector3& other) = default;
        constexpr Vector3& operator = (Vector3&& other) = default;
        constexpr Vector3 operator - () const
        {
            return Vector3{-coord.x, -coord.y, -coord.z};
        }

        Vector3& operator += (const Vector3& other);
        Vector3& operator -= (const Vector3& other);
        Vector3& operator *= (float value);
        friend bool operator == (const Vector3& a, const Vector3& b);
        friend bool operator != (const Vector3& a, const Vector3& b);
        constexpr friend Vector3 operator * (const Vector3& v, float a)
        {
            return Vector3 {v.coord.x * a, v.coord.y * a, v.coord.z * a};
        }

        constexpr friend Vector3 operator * (float a, const Vector3& v)
        {
            return v * a;
        }

        constexpr friend Vector3 operator / (const Vector3& v, float a)
        {
            return Vector3 {v.coord.x / a, v.coord.y / a, v.coord.z / a};
        }

        constexpr friend Vector3 operator * (const Vector3& v1, const Vector3& v2)
        {
            return Vector3 { v1.coord.x * v2.coord.x, v1.coord.y * v2.coord.y, v1.coord.z * v2.coord.z };
        }

        constexpr friend Vector3 operator + (const Vector3& a, const Vector3& b)
        {
            return Vector3 {a.coord.x + b.coord.x, a.coord.y + b.coord.y, a.coord.z + b.coord.z};
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
            struct Coord
            {
                float x;
                float y;
                float z;
            } coord;
        };
    };
}

namespace Feng
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
        coord.x /= norm;
        coord.y /= norm;
        coord.z /= norm;
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
        coord.x += other.coord.x;
        coord.y += other.coord.y;
        coord.z += other.coord.z;

        return *this;
    }

    inline Vector3& Vector3::operator -= (const Vector3& other)
    {
        coord.x -= other.coord.x;
        coord.y -= other.coord.y;
        coord.z -= other.coord.z;

        return *this;
    }

    inline Vector3& Vector3::operator *= (float value)
    {
        coord.x *= value;
        coord.y *= value;
        coord.z *= value;

        return *this;
    }
}
