#pragma once

#include <Feng/ScenesManager/Component.h>

#include <vector>
#include <string>

namespace feng
{
    class Entity final
    {
    public:
        explicit Entity(const std::string& aName);
        explicit Entity(std::string&& aName);
        ~Entity();

        template <class TComponent> TComponent* GetComponent() const;
        template <class TComponent> TComponent& AddComponent();
        template <class TComponent> void RemoveComponent();

    private:
        std::vector<std::unique_ptr<Component>> components;
        std::string name;
    };

    template <class TComponent>
    TComponent* Entity::GetComponent() const
    {
        auto isOfType = [](const std::unique_ptr<Component> &component)
        {
            return dynamic_cast<const TComponent*>(component.get()) != nullptr;
        };

        if(auto it = std::find_if(components.begin(), components.end(), isOfType); it != components.end())
        {
            return static_cast<TComponent*>(it->get());
        }

        return nullptr;
    }

    template <class TComponent>
    TComponent& Entity::AddComponent()
    {
        std::unique_ptr<Component> component = std::make_unique<TComponent>();
        component->SetOwner(this);
        components.push_back(std::move(component));
        return static_cast<TComponent&>(*components.back());
    }

    template <class TComponent>
    void Entity::RemoveComponent()
    {
        auto isOfType = [](const std::unique_ptr<Component> &component)
        {
            return typeid(component.get()) == typeid(TComponent);
        };

        components.erase(std::remove_if(components.begin(), components.end(), isOfType));
    }
}
