#pragma once

#include <Feng/Math/Vector3.h>

namespace Feng
{
    class Vector4 final
    {
    public:
        Vector4() = default;
        constexpr Vector4(float aX, float aY, float aZ, float aW)
            : data {aX, aY, aZ, aW}
        { }
        constexpr Vector4(const Vector3& aXyz, float aW)
            : data {aXyz.coord.x, aXyz.coord.y, aXyz.coord.z, aW}
        { }
        constexpr Vector4(const Vector3& aXyz)
            : Vector4(aXyz, 0.f)
        { }
        
        constexpr Vector4(const Vector4&) = default;
        constexpr Vector4(Vector4&&) = default;

        constexpr Vector3 GetXyz() const
        {
            return Vector3{ coord.x, coord.y, coord.z};
        }

        constexpr Vector4& operator = (const Vector4& other) = default;
        constexpr Vector4& operator = (Vector4&& other) = default;
        constexpr Vector4 operator - () const
        {
            return Vector4{-coord.x, -coord.y, -coord.z, -coord.w};
        }
        
        constexpr friend Vector4 operator * (const Vector4& v, float a)
        {
            return Vector4 {v.coord.x * a, v.coord.y * a, v.coord.z * a, v.coord.w * a};
        }

        constexpr friend Vector4 operator * (float a, const Vector4& v)
        {
            return v * a;
        }

        constexpr friend Vector4 operator / (const Vector4& v, float a)
        {
            return Vector4 {v.coord.x / a, v.coord.y / a, v.coord.z / a, v.coord.w / a};
        }

        constexpr friend Vector4 operator * (const Vector4& v1, const Vector4& v2)
        {
            return Vector3 { v1.coord.x * v2.coord.x, v1.coord.y * v2.coord.y, v1.coord.z * v2.coord.z };
        }

        constexpr friend Vector4 operator + (const Vector4& a, const Vector4& b)
        {
            return Vector3 {a.coord.x + b.coord.x, a.coord.y + b.coord.y, a.coord.z + b.coord.z};
        }

        constexpr friend Vector4 operator - (const Vector4& a, const Vector4& b)
        {
            return a + (-b);
        }
        
        struct Coord
        {
            float x;
            float y;
            float z;
            float w;
        };

        union
        {
            float data[4];
            Coord coord;
        };
    };
}
