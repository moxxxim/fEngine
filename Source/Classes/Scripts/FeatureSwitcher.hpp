#pragma once

#include <Feng/ScenesManager/Component.h>
#include <cstdint>

class FeatureSwitcher final : public Feng::Component
{
public:
    FeatureSwitcher() = default;
    
private:
    void Update(float deltaTime) override;
    
    void SwitchPostEffect();
    void SwitchShadowMapCascade();
    bool CheckTimeout();
    int SwitchValues(int value, int max, int last) const;
    
    constexpr static float KeyPressTimeOut = 0.3f;
    
    float switcherInputDelay = 0.3f;
    float lastInput = 0.f;
    int32_t appliedEffectIndex = -1;
    int32_t appliedCascade = -1;
};
