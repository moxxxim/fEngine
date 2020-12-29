#include <Feng/Render/PostEffects/PostEffectDefinition.h>

#include <Feng/ResourcesManager/Material.h>
#include <Feng/Render/PostEffects/PostEffectPassBehaviour.h>

namespace feng
{
    PostEffectDefinition::PostEffectDefinition(std::unique_ptr<Material>&& aMaterial)
        : type(PostEffectType::Material)
        , material(std::move(aMaterial))
    { }

    PostEffectDefinition::PostEffectDefinition(
                                        std::unique_ptr<Material>&& aMaterial,
                                        std::unique_ptr<PostEffectPassBehaviour>&& aPassBehaviour)
        : type(PostEffectType::Material)
        , material(std::move(aMaterial))
        , passBehaviour(std::move(aPassBehaviour))
    { }

    PostEffectDefinition::PostEffectDefinition(std::vector<std::unique_ptr<PostEffectDefinition>>&& aSubsequence)
        : type(PostEffectType::Composite)
        , subsequence(std::move(aSubsequence))
    { }

    PostEffectDefinition::~PostEffectDefinition() = default;
}
