#pragma once

namespace fengine
{
    class Entity;
    class Transform;

    class Component
    {
        friend Entity;
    public:
        const Entity* GetEntity() const;
        Entity* GetEntity();

    protected:
        Component() = default;
        virtual ~Component() = default;

        void SetOwner(Entity *owner);

        Entity *m_owner = nullptr;
    };

    inline const Entity* Component::GetEntity() const
    {
        return m_owner;
    }

    inline Entity* Component::GetEntity()
    {
        return m_owner;
    }

    inline void Component::SetOwner(Entity *owner)
    {
        m_owner = owner;
    }
}
