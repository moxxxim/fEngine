#pragma once

#include <Feng/ScenesManager/Component.h>
#include <cstdint>

class PostEffectSwitcher final : public feng::Component
{
public:
    PostEffectSwitcher() = default;
    
private:
    void Update(float deltaTime) override;
    
    void ApplyNextPostEffect();
    
    float effectChangeInputDelay = 0.3f;
    float lastEffectChangeTime = 0.f;
    int32_t appliedEffectIndex = -1;
};
