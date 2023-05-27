#include <Classes/Scripts/FeatureSwitcher.hpp>

#include <Classes/TempResouces.h>
#include <Feng/Core/Engine.hpp>
#include <Feng/ScenesManager/RenderSystem.h>
#include <Feng/Utils/Debug.h>
#include <Feng/Utils/Render/ShaderParams.h>
#include <GLFW/glfw3.h>

void FeatureSwitcher::Update(float deltaTime)
{
    Feng::Engine *engine = Feng::Engine::Instance();
    if(appliedEffectIndex >= 0)
    {
        Feng::PostEffectDefinition *postEffect = test::res.Effects[appliedEffectIndex];
        engine->SetPostEffect(postEffect);
    }
    else
    {
        engine->RemovePostEffect();
    }
    
    Feng::Engine::SetDirectShowDebugShadowMap(appliedCascade != -1);
    if(appliedCascade != -1)
    {
        Feng::RenderSystem *renderSystem = engine->GetCurrentScene()->GetRenderSystem();
        renderSystem->GlobalBindings().SetInt(Feng::ShaderParams::DebugCascadeNum.data(), appliedCascade);
    }
    
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_O))
    {
        if(CheckTimeout())
        {
            SwitchPostEffect();
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_I))
    {
        if(CheckTimeout())
        {
            Feng::Engine::SetMultisampleEnabled(!Feng::Engine::IsMultisampleEnabled());
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_B))
    {
        if(CheckTimeout())
        {
            SwitchShadowMapCascade();
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_N))
    {
        if(CheckTimeout())
        {
            Feng::Engine::SetOmniShowDebugShadowMap(!Feng::Engine::IsOmniShowDebugShadowMap());
        }
    }

    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_C))
    {
        if(CheckTimeout())
        {
            Feng::Engine::SetCursorVisible(!Feng::Engine::IsCursorVisible());
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_H))
    {
        if(CheckTimeout())
        {
            Feng::RenderSystem *renderSystem = engine->GetCurrentScene()->GetRenderSystem();
            renderSystem->SetHdr(!renderSystem->IsHdr());
        }
    }
}

void FeatureSwitcher::SwitchPostEffect()
{
    appliedEffectIndex = SwitchValues(appliedEffectIndex, static_cast<int32_t>(test::res.Effects.size()), -1);
}

void FeatureSwitcher::SwitchShadowMapCascade()
{
    constexpr int32_t cascadesCount = 3;
    appliedCascade = SwitchValues(appliedCascade, cascadesCount, -1);
}

bool FeatureSwitcher::CheckTimeout()
{
    float time = Feng::Engine::Time();
    if(Feng::Engine::Time() - lastInput > switcherInputDelay)
    {
        lastInput = time;
        return true;
    }

    return false;
}

int FeatureSwitcher::SwitchValues(int value, int max, int last) const
{
    return (value + 1 < max) ? value + 1 : last;
}
