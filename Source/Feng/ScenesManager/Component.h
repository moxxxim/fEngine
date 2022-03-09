#pragma once

namespace Feng
{
    class Entity;
    
    class Component
    {
        friend Entity;
    public:
        virtual ~Component() = default;
        const Entity* GetEntity() const;
        Entity* GetEntity();

    protected:
        Component() = default;
        virtual void Update(float deltaTime);

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
    
    inline void Component::Update(float deltaTime)
    {}
    
    inline void Component::SetOwner(Entity *aOwner)
    {
        owner = aOwner;
    }
}
