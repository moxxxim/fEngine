#include <Feng/ScenesManager/Entity.h>

#include <Feng/ScenesManager/Transform.h>

#include <algorithm>

namespace feng
{
    Entity::Entity()
        : Entity {""}
    { }

    Entity::Entity(const std::string& aName)
        : name {aName}
    {
        std::ignore = AddComponent<Transform>();
    }

    Entity::Entity(std::string&& aName)
        : name {std::move(aName)}
    {
        std::ignore = AddComponent<Transform>();
    }

    Entity::~Entity() = default;
    
    void Entity::Update(float deltaTime)
    {
        std::for_each(
                      components.begin(),
                      components.end(),
                      [deltaTime](std::unique_ptr<Component>& component) { component->Update(deltaTime); });
    }
}
