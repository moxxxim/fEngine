#include <Classes/Scripts/PostEffectSwitcher.hpp>

#include <Classes/TempResouces.h>
#include <Feng/Core/Engine.hpp>

void PostEffectSwitcher::Update(float deltaTime)
{
    if (feng::Engine::IsKeyPressed(feng::InputKey::Kb_O))
    {
        ApplyNextPostEffect();
    }

    if(appliedEffectIndex >= 0)
    {
        feng::PostEffectDefinition *postEffect = test::res.Effects[appliedEffectIndex];
        feng::Engine::Instance()->SetPostEffect(postEffect);
    }
    else
    {
        feng::Engine::Instance()->RemovePostEffect();
    }
}

void PostEffectSwitcher::ApplyNextPostEffect()
{
    float time = feng::Engine::Time();
    if(feng::Engine::Time() - lastEffectChangeTime > effectChangeInputDelay)
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
