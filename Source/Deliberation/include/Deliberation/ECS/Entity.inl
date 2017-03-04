#include <memory>
#include <type_traits>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/Component.h>
#include <Deliberation/ECS/ComponentWrapper.h>

namespace deliberation
{

template<typename T>
bool Entity::hasComponent() const
{
    static_assert(std::is_base_of<ComponentBase, T>::value, "");
    return hasComponent(T::indexStatic());
}

template<typename T>
T & Entity::component()
{
    static_assert(std::is_base_of<ComponentBase, T>::value, "");

    return dynamic_cast<T&>(component(T::indexStatic()));
}

template<typename T>
const T & Entity::component() const
{
    static_assert(std::is_base_of<ComponentBase, T>::value, "");

    return dynamic_cast<T&>(component(T::indexStatic())).value();
}

template<typename T, typename ... Args>
T & Entity::addComponent(Args &&... args)
{
    static_assert(std::is_base_of<ComponentBase, T>::value, "");

    auto * ptr = (T*)nullptr;

    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    ptr = component.get();
    addComponent(ptr->index(), std::move(component));

    return *ptr;
}

template<typename T>
void Entity::removeComponent()
{
    static_assert(std::is_base_of<ComponentBase, T>::value, "");

    removeComponent(T::indexStatic());
}

}

