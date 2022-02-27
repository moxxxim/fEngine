#pragma once

#include <Feng/Math/MathUtils.h>
#include <Feng/Math/Matrix3.h>
#include <Feng/Math/Vector3.h>
#include <Feng/Math/Vector4.h>
#include <array>

namespace feng
{
    class Quaternion final
    {
    public:
        static constexpr Quaternion Identity()
        {
            return Quaternion{0.f, 0.f, 0.f, 1.f};
        }

        Quaternion() = default;
        Quaternion(const Vector3& normAxis, float dAngle)
        {
            float radHalfAngle = DegToRad(dAngle / 2);
            float sine = std::sin(radHalfAngle);
            xyzw[0] = sine * normAxis.x;
            xyzw[1] = sine * normAxis.y;
            xyzw[2] = sine * normAxis.z;
            xyzw[3] = std::cos(radHalfAngle);
        }

        constexpr explicit Quaternion(const Vector4& aXyzw)
            : Quaternion {aXyzw.x, aXyzw.y, aXyzw.z, aXyzw.w}
        {}
        
        constexpr Quaternion(float x, float y, float z, float w)
            : xyzw {x, y, z, w}
        { }

        constexpr Quaternion(const Quaternion& other) = default;
        constexpr Quaternion(Quaternion&& other) = default;

        constexpr Quaternion Inverse() const
        {
            return Quaternion {-xyzw[0], -xyzw[1], -xyzw[2], xyzw[3]};
        }

        constexpr Vector3 AxisScaled() const
        {
            return Vector3 {xyzw[0], xyzw[1], xyzw[2]};
        }
        
        constexpr Vector3 Axis() const
        {
            return Vector3 {xyzw[0], xyzw[1], xyzw[2]} / (1 - xyzw[3] * xyzw[3]);
        }
        
        constexpr Matrix3 ToMatrix3() const
        {
            /*
               [ 1-2(y^2 + z^2)   2(xy + wz)         2(xz - wy)         ]
               [ 2(xy - wz)       1 - 2(x^2 + z^2)   2(yz + wx)         ]
               [ 2(xz + wy)       2(yz - wx)         1 - 2(x^2 + y^2)   ]
             */
            
            const float x = xyzw[0];
            const float y = xyzw[1];
            const float z = xyzw[2];
            const float w = xyzw[3];

            const Vector3 row1 {
                1.f - 2.f * (y*y + z*z),
                2.f * (x*y + z*w),
                2.f * (x*z - y*w) };

            const Vector3 row2 {
                2.f * (x*y - z*w),
                1.f - 2.f * (x*x + z*z),
                2 * (y*z + x*w)};

            const Vector3 row3 {
                2.f * (x*z + y*w),
                2.f * (y*z - x*w),
                1.f - 2.f * (x*x + y*y)};

            return Matrix3::Rows(row1, row2, row3);
        }

        constexpr Quaternion& operator = (const Quaternion& other) = default;
        constexpr Quaternion& operator = (Quaternion&& other) = default;

        friend constexpr Quaternion operator * (const Quaternion& lhs, const Quaternion& rhs)
        {
            const Vector3 vLhs = lhs.AxisScaled();
            const float wLhs = lhs.xyzw[3];
            
            const Vector3 vRhs = rhs.AxisScaled();
            const float wRhs = rhs.xyzw[3];

            const Vector3 outAxisScaled = Vector3::Cross(vLhs, vRhs) + wLhs * vRhs + wRhs * vLhs;
            const float outW = wLhs * wRhs - Vector3::Dot(vLhs, vRhs);

            return Quaternion{outAxisScaled.x, outAxisScaled.y, outAxisScaled.z, outW};
        }
        
        friend constexpr Vector3 operator * (const Vector3& point, const Quaternion& rotation)
        {
            // Full wormula:
            const Quaternion pointAsQuaternion {point.x, point.y, point.z, 0};
            const Quaternion multAsQuaternion = rotation * pointAsQuaternion * rotation.Inverse();
            return multAsQuaternion.AxisScaled();

            const Vector3 axis = rotation.AxisScaled();
            const float cosHalfAngle = rotation.xyzw[3];

            return 2 * (cosHalfAngle * Vector3::Cross(axis, point))
                 + (cosHalfAngle * cosHalfAngle) * point
                 + axis * Vector3::Dot(axis, point);
        }
        
        Quaternion operator *= (const Quaternion& other);

    private:

        std::array<float, 4> xyzw;
    };
    
    inline Quaternion Quaternion::operator *= (const Quaternion& other)
    {
        *this = *this * other;
        return *this;
    }
}
