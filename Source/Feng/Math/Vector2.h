#pragma once

#include <cmath>
#include <cstring>

namespace Feng
{
    class Vector2 final
    {
    public:
        static float Dot(const Vector2& a, const Vector2& b);
        static float Distance(const Vector2& a, const Vector2& b);
        static Vector2 Lerp(const Vector2& a, const Vector2& b, float t);

        Vector2() = default;
        Vector2(float aX, float aY);
        Vector2(const Vector2&) = default;
        Vector2(Vector2&&) = default;

        float Length() const;
        float LengthSqr() const;
        Vector2 ProjectOn(const Vector2& on) const;
        Vector2 ToNormalized() const;
        void Normalize();

        Vector2& operator = (const Vector2& other) = default;
        Vector2& operator = (Vector2&& other) = default;
        Vector2 operator - () const;
        Vector2& operator += (const Vector2& other);
        Vector2& operator -= (const Vector2& other);
        Vector2& operator *= (float value);

        friend bool operator == (const Vector2& a, const Vector2& b);
        friend bool operator != (const Vector2& a, const Vector2& b);
        friend Vector2 operator * (const Vector2& v, float a);
        friend Vector2 operator * (float a, const Vector2& v);
        friend Vector2 operator / (const Vector2& v, float a);
        friend Vector2 operator + (const Vector2& a, const Vector2& b);
        friend Vector2 operator - (const Vector2& a, const Vector2& b);

        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 OneX;
        static const Vector2 OneY;

        union
        {
            float data[2];
            struct Coord
            {
                float x;
                float y;
            } coord;
        };
    };
}

namespace Feng
{
    inline Vector2::Vector2(float aX, float aY)
        : data {aX, aY}
    {}

    inline float Vector2::Length() const
    {
        return std::sqrt(Vector2::LengthSqr());
    }

    inline float Vector2::LengthSqr() const
    {
        return Vector2::Dot(*this, *this);
    }

    inline Vector2 Vector2::ProjectOn(const Vector2& on) const
    {
        return on * Vector2::Dot(*this, on);
    }

    inline Vector2 Vector2::ToNormalized() const
    {
        Vector2 normalized = *this;
        normalized.Normalize();

        return normalized;
    }

    inline void Vector2::Normalize()
    {
        float norm = Length();
        coord.x /= norm;
        coord.y /= norm;
    }

    inline float Vector2::Dot(const Vector2& a, const Vector2& b)
    {
        return (a.coord.x * b.coord.x) + (a.coord.y * b.coord.y);
    }

    inline float Vector2::Distance(const Vector2& a, const Vector2& b)
    {
        return (b - a).Length();
    }

    inline Vector2 Vector2::Lerp(const Vector2& a, const Vector2& b, float t)
    {
        return ((1 - t) * a) + (t * b);
    }

    inline Vector2 Vector2::operator - () const
    {
        return Vector2{-coord.x, -coord.y};
    }

    inline Vector2& Vector2::operator += (const Vector2& other)
    {
        coord.x += other.coord.x;
        coord.y += other.coord.y;

        return *this;
    }

    inline Vector2& Vector2::operator -= (const Vector2& other)
    {
        coord.x -= other.coord.x;
        coord.y -= other.coord.y;

        return *this;
    }

    inline Vector2& Vector2::operator *= (float value)
    {
        coord.x *= value;
        coord.y *= value;

        return *this;
    }

    inline bool operator == (const Vector2& a, const Vector2& b)
    {
        return (std::memcmp(a.data, b.data, sizeof(Vector2)) == 0);
    }

    inline bool operator != (const Vector2& a, const Vector2& b)
    {
        return !(a == b);
    }

    inline Vector2 operator * (const Vector2& v, float a)
    {
        return Vector2 {v.coord.x * a, v.coord.y * a};
    }

    inline Vector2 operator * (float a, const Vector2& v)
    {
        return v * a;
    }

    inline Vector2 operator / (const Vector2& v, float a)
    {
        return Vector2 {v.coord.x / a, v.coord.y / a};
    }

    inline Vector2 operator + (const Vector2& a, const Vector2& b)
    {
        return Vector2 {a.coord.x + b.coord.x, a.coord.y + b.coord.y};
    }

    inline Vector2 operator - (const Vector2& a, const Vector2& b)
    {
        return a + (-b);
    }
}

