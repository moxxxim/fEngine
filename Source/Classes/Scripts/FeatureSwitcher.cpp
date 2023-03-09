#include <Classes/Scripts/FeatureSwitcher.hpp>

#include <Classes/TempResouces.h>
#include <Feng/Core/Engine.hpp>
#include <GLFW/glfw3.h>

void FeatureSwitcher::Update(float deltaTime)
{
    if (Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_O))
    {
        ApplyNextPostEffect();
    }

    if(appliedEffectIndex >= 0)
    {
        Feng::PostEffectDefinition *postEffect = test::res.Effects[appliedEffectIndex];
        Feng::Engine::Instance()->SetPostEffect(postEffect);
    }
    else
    {
        Feng::Engine::Instance()->RemovePostEffect();
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_I))
    {
        if(!isDelayActive)
        {
            Feng::Engine::SetMultisampleEnabled(!Feng::Engine::IsMultisampleEnabled());
            isDelayActive = true;
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_B))
    {
        if(!isDelayActive)
        {
            Feng::Engine::SetDirectShowDebugShadowMap(!Feng::Engine::IsDirectShowDebugShadowMap());
            isDelayActive = true;
        }
    }
    
    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_N))
    {
        if(!isDelayActive)
        {
            Feng::Engine::SetOmniShowDebugShadowMap(!Feng::Engine::IsOmniShowDebugShadowMap());
            isDelayActive = true;
        }
    }

    if(Feng::Engine::IsKeyPressed(Feng::InputKey::Kb_C))
    {
        if(!isDelayActive)
        {
            Feng::Engine::SetCursorVisible(!Feng::Engine::IsCursorVisible());
            isDelayActive = true;
        }
    }

    if(isDelayActive)
    {
        delay += deltaTime;
        if(delay > KeyPressTimeOut)
        {
            delay = 0.f;
            isDelayActive = false;
        }
    }
}

void FeatureSwitcher::ApplyNextPostEffect()
{
    float time = Feng::Engine::Time();
    if(Feng::Engine::Time() - lastEffectChangeTime > effectChangeInputDelay)
    {
        lastEffectChangeTime = time;
        if(appliedEffectIndex + 1 < test::res.Effects.size())
        {
            ++appliedEffectIndex;
        }
        else
        {
            appliedEffectIndex = -1;
        }
    }
}
