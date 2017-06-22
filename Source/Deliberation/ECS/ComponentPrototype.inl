#pragma once

namespace deliberation
{

template<typename ComponentT>
void ComponentPrototype<ComponentT>::updateEntity(Entity entity)
{
    if (!entity.hasComponent<ComponentT>())
    {
        addComponentToEntity(entity);
    }

    auto & component = entity.component<ComponentT>();

    updateComponent(component);
}

}