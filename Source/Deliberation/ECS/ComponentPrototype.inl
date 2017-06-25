#pragma once

namespace deliberation
{

template<typename ComponentT>
bool ComponentPrototype<ComponentT>::hasComponent(const Entity & entity)
{
    return entity.hasComponent<ComponentT>();
}

template<typename ComponentT>
void ComponentPrototype<ComponentT>::onUpdateEntity(Entity entity)
{
    if (!entity.hasComponent<ComponentT>())
    {
        addComponentToEntity(entity);
    }

    auto & component = entity.component<ComponentT>();

    updateComponent(component);
}

}