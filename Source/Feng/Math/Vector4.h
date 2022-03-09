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
