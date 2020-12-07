#include <FEngine/ScenesManager/Entity.h>

namespace fengine
{
    Entity::~Entity()
    {

    }

    void Entity::AddComponent(Component *component)
    {
    }

    void Entity::RemoveComponent(Component *component)
    {
    }

    void Entity::Update(float deltaTime)
    {

    }

    void Entity::DeleteComponents()
    {
        for(Component* component: components)
        {
            delete component;
        }

        components.clear();
    }
}
