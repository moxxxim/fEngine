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
            : data {aXyz.x, aXyz.y, aXyz.z, aW}
        { }
        constexpr Vector4(const Vector3& aXyz)
            : Vector4(aXyz, 0.f)
        { }
        
        constexpr Vector4(const Vector4&) = default;
        constexpr Vector4(Vector4&&) = default;

        constexpr Vector3 GetXyz() const
        {
            return Vector3{x, y, z};
        }

        constexpr Vector4& operator = (const Vector4& other) = default;
        constexpr Vector4& operator = (Vector4&& other) = default;
        constexpr Vector4 operator - () const
        {
            return Vector4{-x, -y, -z, -w};
        }
        
        constexpr friend Vector4 operator * (const Vector4& v, float a)
        {
            return Vector4 {v.x * a, v.y * a, v.z * a, v.w * a};
        }

        constexpr friend Vector4 operator * (float a, const Vector4& v)
        {
            return v * a;
        }

        constexpr friend Vector4 operator / (const Vector4& v, float a)
        {
            return Vector4 {v.x / a, v.y / a, v.z / a, v.w / a};
        }

        constexpr friend Vector4 operator * (const Vector4& v1, const Vector4& v2)
        {
            return Vector3 { v1.x * v2.x, v1.y * v2.y, v1.z * v2.z };
        }

        constexpr friend Vector4 operator + (const Vector4& a, const Vector4& b)
        {
            return Vector3 {a.x + b.x, a.y + b.y, a.z + b.z};
        }

        constexpr friend Vector4 operator - (const Vector4& a, const Vector4& b)
        {
            return a + (-b);
        }

        union
        {
            float data[4];
            struct
            {
                float x;
                float y;
                float z;
                float w;
            };
        };
    };
}
