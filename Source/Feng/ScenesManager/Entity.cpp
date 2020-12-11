#include <Feng/ScenesManager/Entity.h>

#include <Feng/ScenesManager/Transform.h>

namespace feng
{
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
}
