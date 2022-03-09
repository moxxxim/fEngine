#include <Classes/Scripts/PostEffectSwitcher.hpp>

#include <Classes/TempResouces.h>
#include <Feng/Core/Engine.hpp>

void PostEffectSwitcher::Update(float deltaTime)
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
}

void PostEffectSwitcher::ApplyNextPostEffect()
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
