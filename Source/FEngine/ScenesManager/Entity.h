#pragma once

#include <FEngine/ScenesManager/Component.h>

#include <vector>

namespace fengine
{
    class Component;

    class Entity final
    {
    public:
        Entity();
        ~Entity();
        
        const Transform& GetTransform() const;
        Transform& GetTransform();

        template <class TComponent> TComponent* GetComponent() const;
        template <class TComponent> std::vector<TComponent*> GetComponents() const;
        template <class TComponent> TComponent* AddComponent();
        template <class TComponent> void RemoveComponent();

        void RemoveComponent(Component *component);

    private:
        void DeleteComponents();
        void Update(float deltaTime);
        void AddComponent(Component* component);

        std::vector<Component*> components;
    };

    template <class TComponent>
    TComponent* Entity::GetComponent() const
    {
        return nullptr;
    }

    template <class TComponent>
    std::vector<TComponent*> Entity::GetComponents() const
    {
        return {};
    }

    template <class TComponent>
    TComponent* Entity::AddComponent()
    {
        return nullptr;
    }

    template <class TComponent>
    void Entity::RemoveComponent()
    {

    }
}
