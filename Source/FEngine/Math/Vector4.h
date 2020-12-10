#pragma once

#include <FEngine/Math/Vector3.h>

namespace feng
{
    class Vector4 final
    {
    public:
        Vector4() = default;
        Vector4(float aX, float aY, float aZ, float aW);
        Vector4(const Vector3& aXyz, float aW);
        Vector4(const Vector4&) = default;
        Vector4(Vector4&&) = default;

        Vector3 GetXyz() const;

        Vector4& operator = (const Vector4& other) = default;
        Vector4& operator = (Vector4&& other) = default;

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

namespace feng
{
    inline Vector4::Vector4(float aX, float aY, float aZ, float aW)
        : data {aX, aY, aZ, aW}
    { }

    inline Vector4::Vector4(const Vector3& aXyz, float aW)
        : data {aXyz.x, aXyz.y, aXyz.z, aW}
    { }

    inline Vector3 Vector4::GetXyz() const
    {
        return Vector3 {x, y, z};
    }
}
