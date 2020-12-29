#pragma once

#include <Feng/Render/PostEffects/PostEffectType.h>

#include <vector>

namespace feng
{
    class Material;
    class PostEffectPassBehaviour;

    class PostEffectDefinition final
    {
    public:
        PostEffectDefinition(std::unique_ptr<Material>&& aMaterial);
        PostEffectDefinition(
                        std::unique_ptr<Material>&& aMaterial,
                        std::unique_ptr<PostEffectPassBehaviour>&& aPassBehaviour);
        PostEffectDefinition(std::vector<std::unique_ptr<PostEffectDefinition>>&& aSubsequence);
        ~PostEffectDefinition();

        PostEffectType GetType() const;
        Material* GetMaterial();
        PostEffectPassBehaviour* GetPassBehaviour();
        const std::vector<std::unique_ptr<PostEffectDefinition>>& GetSubsequence() const;

    private:
        PostEffectType type = PostEffectType::Material;
        std::unique_ptr<Material> material;
        std::unique_ptr<PostEffectPassBehaviour> passBehaviour;
        std::vector<std::unique_ptr<PostEffectDefinition>> subsequence;
    };
}

namespace feng
{
    inline PostEffectType PostEffectDefinition::GetType() const
    {
        return type;
    }

    inline Material* PostEffectDefinition::GetMaterial()
    {
        return material.get();
    }

    inline PostEffectPassBehaviour* PostEffectDefinition::GetPassBehaviour()
    {
        return passBehaviour.get();
    }

    inline const std::vector<std::unique_ptr<PostEffectDefinition>>& PostEffectDefinition::GetSubsequence() const
    {
        return subsequence;
    }
}
