#pragma once

#include <FEngine/App/Object.h>
#include <FEngine/ResourcesManager/PostEffectType.h>
#include <FEngine/ResourcesManager/CachedPostEffectDefinition.h>


#include <vector>

namespace fengine
{
    class Material;
    class PostEffectPassBehaviour;

    class PostEffectDefinition final: public Object
    {
    public:
        PostEffectDefinition(Material *material, PostEffectPassBehaviour *passBehaviour);
        PostEffectDefinition(const std::vector<PostEffectDefinition*>& innerSequence);
        ~PostEffectDefinition();

        void Destroy() override;

        PostEffectType GetType() const;
        Material * GetMaterial() const;
        PostEffectPassBehaviour * GetPassBehaviour() const;
        const std::vector<PostEffectDefinition*>& GetInnerSequence() const;

    private:
        PostEffectType m_type = PostEffectType::Material;
        Material *m_material = nullptr;
        PostEffectPassBehaviour *m_passBehaviour = nullptr;
        std::vector<PostEffectDefinition*> m_innerSequence;
};

    inline PostEffectType PostEffectDefinition::GetType() const
    {
        return m_type;
    }

    inline Material * PostEffectDefinition::GetMaterial() const
    {
        return m_material;
    }

    inline PostEffectPassBehaviour * PostEffectDefinition::GetPassBehaviour() const
    {
        return m_passBehaviour;
    }

    inline const std::vector<PostEffectDefinition*>& PostEffectDefinition::GetInnerSequence() const
    {
        return m_innerSequence;
    }
}
