#include <FEngine/ScenesManager/Entity.h>

#include <FEngine/ScenesManager/Transform.h>

namespace fengine
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
