#pragma once

#include <Feng/ScenesManager/Component.h>
#include <cstdint>

class FeatureSwitcher final : public Feng::Component
{
public:
    FeatureSwitcher() = default;
    
private:
    void Update(float deltaTime) override;
    
    void ApplyNextPostEffect();
    
    constexpr static float MultisampleSwitchTimeOut = 0.25f;
    
    float effectChangeInputDelay = 0.3f;
    float lastEffectChangeTime = 0.f;
    int32_t appliedEffectIndex = -1;
    float delay = 0.25;
    bool isDelayActive = false;
};
