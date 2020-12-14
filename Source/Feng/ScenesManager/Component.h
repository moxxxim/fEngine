#pragma once

namespace feng
{
    class Entity;
    class Transform;

    class Component
    {
        friend Entity;
    public:
        virtual ~Component() = default;
        const Entity* GetEntity() const;
        Entity* GetEntity();

    protected:
        Component() = default;

        void SetOwner(Entity *aOwner);

        Entity *owner = nullptr;
    };

    inline const Entity* Component::GetEntity() const
    {
        return owner;
    }

    inline Entity* Component::GetEntity()
    {
        return owner;
    }

    inline void Component::SetOwner(Entity *aOwner)
    {
        owner = aOwner;
    }
}
