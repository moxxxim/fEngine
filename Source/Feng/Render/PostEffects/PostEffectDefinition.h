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
        PostEffectDefinition(Material *material, PostEffectPassBehaviour *passBehaviour);
        PostEffectDefinition(const std::vector<PostEffectDefinition*>& innerSequence);
        ~PostEffectDefinition();

        PostEffectType GetType() const;
        Material * GetMaterial() const;
        PostEffectPassBehaviour * GetPassBehaviour() const;
        const std::vector<PostEffectDefinition*>& GetInnerSequence() const;

    private:
        PostEffectType type = PostEffectType::Material;
        Material *material = nullptr;
        PostEffectPassBehaviour *passBehaviour = nullptr;
        std::vector<PostEffectDefinition*> subsequence;
    };
}

namespace feng
{
    inline PostEffectType PostEffectDefinition::GetType() const
    {
        return type;
    }

    inline Material * PostEffectDefinition::GetMaterial() const
    {
        return material;
    }

    inline PostEffectPassBehaviour * PostEffectDefinition::GetPassBehaviour() const
    {
        return passBehaviour;
    }

    inline const std::vector<PostEffectDefinition*>& PostEffectDefinition::GetInnerSequence() const
    {
        return subsequence;
    }
}
