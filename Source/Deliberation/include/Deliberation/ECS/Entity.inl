#include <memory>
#include <type_traits>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/Component.h>

namespace deliberation
{

template<typename T>
bool Entity::hasComponent() const
{
    Assert((std::is_base_of<Component<T>, T>::value), "");

    return hasComponent(T::indexStatic());
}

template<typename T>
T & Entity::component()
{
    Assert((std::is_base_of<Component<T>, T>::value), "");

    auto * ptr = (T*)component(T::indexStatic());
    Assert(ptr, "");

    return *ptr;
}

template<typename T>
const T & Entity::component() const
{
    Assert((std::is_base_of<Component<T>, T>::value), "");

    auto * ptr = (T*)component(T::indexStatic());
    Assert(ptr, "");

    return *ptr;
}

template<typename T, typename ... Args>
T & Entity::addComponent(Args &&... args)
{
    Assert((std::is_base_of<Component<T>, T>::value), "");

    auto * ptr = new T(std::forward<Args>(args)...);

    addComponent(T::indexStatic(), ptr);

    return *ptr;
}

template<typename T>
void Entity::removeComponent()
{
    Assert((std::is_base_of<Component<T>, T>::value), "");

    removeComponent(T::indexStatic());
}

}

