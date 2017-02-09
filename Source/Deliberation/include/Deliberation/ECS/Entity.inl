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
    return hasComponent(ComponentWrapper<T>::indexStatic());
}

template<typename T>
T & Entity::component()
{
    auto * ptr = (T*)component(ComponentWrapper<T>::indexStatic());
    Assert(ptr, "");

    return *ptr;
}

template<typename T>
const T & Entity::component() const
{
    auto * ptr = (T*)component(ComponentWrapper<T>::indexStatic());
    Assert(ptr, "");

    return *ptr;
}

template<typename T, typename ... Args>
T & Entity::addComponent(Args &&... args)
{
    auto component = std::make_unique<ComponentWrapper<T>>(std::forward<Args>(args)...);
    auto & ref = *component;

    addComponent(ComponentWrapper<T>::indexStatic(), std::move(component));

    return ref.value();
}

template<typename T>
void Entity::removeComponent()
{
    removeComponent(ComponentWrapper<T>::indexStatic());
}

}

