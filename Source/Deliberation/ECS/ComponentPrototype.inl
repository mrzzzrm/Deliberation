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
        auto & component = entity.addComponent<ComponentT>();
        try
        {
            initComponent(component);
        }
        catch(const std::exception & e)
        {
            std::cout << "ComponentPrototype '" << name() << "': Error initializing Component: " << e.what() << "\nJSON:\n" << m_json << std::endl;
        }
    }

    auto & component = entity.component<ComponentT>();

    try
    {
        updateComponent(component);
    }
    catch(const std::exception & e)
    {
        std::cout << "ComponentPrototype: Error updating Component: " << e.what() << "\nJSON:\n" << m_json << std::endl;
    }
}

}