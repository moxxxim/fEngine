#pragma once

#include <Feng/Math/Vector4.h>
#include <Feng/ScenesManager/Component.h>

#include <cstdint>

namespace feng
{
    class Light final : public Component
    {
    public:
        enum class eType : uint8_t
        {
            Directional = 0,
            Point = 1,
            Spot = 2,
        };

        Light();
        ~Light() = default;

        eType GetType() const;
        void SetType(eType aType);

        Vector4 GetColor() const;
        void SetColor(const Vector4& aColor);

        float GetIntesity() const;
        void SetIntensity(float aIntencity);

        float GetRange() const;
        void SetRange(float aRange);

        float GetSpotAngle() const;
        void SetSpotAngle(float aSpotAngle);

    private:
        Vector4 color;
        float intensity = 1.f;
        float range = 1.f;
        float spotAngle = 20.f;
        eType type = eType::Directional;
    };
}

namespace feng
{
    inline Light::eType Light::GetType() const
    {
        return type;
    }

    inline void Light::SetType(Light::eType aType)
    {
        type = aType;
    }

    inline Vector4 Light::GetColor() const
    {
        return color;
    }

    inline void Light::SetColor(const Vector4& aColor)
    {
        color = aColor;
    }

    inline float Light::GetIntesity() const
    {
        return intensity;
    }

    inline void Light::SetIntensity(float aIntensity)
    {
        intensity = aIntensity;
    }

    inline float Light::GetRange() const
    {
        return range;
    }

    inline void Light::SetRange(float aRange)
    {
        range = aRange;
    }

    inline float Light::GetSpotAngle() const
    {
        return spotAngle;
    }

    inline void Light::SetSpotAngle(float aSpotAngle)
    {
        spotAngle = aSpotAngle;
    }
}
