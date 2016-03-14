#include <memory>
#include <type_traits>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/Component.h>

namespace deliberation
{

template<typename T>
bool Entity::hasComponent() const
{
    Assert(std::is_base_of<Component<T>, T>::value, "");
    return m_world.component(m_id, T::index()) != nullptr;
}

template<typename T>
T & Entity::component()
{
    Assert(std::is_base_of<Component<T>, T>::value, "");

    auto * ptr = (T*)m_world.component(m_id, T::index());
    Assert(ptr, "");

    return *ptr;
}

template<typename T>
const T & Entity::component() const
{
    Assert(std::is_base_of<Component<T>, T>::value, "");

    auto * ptr = (const T*)m_world.component(m_id, T::index());
    Assert(ptr, "");

    return *ptr;
}

template<typename T, typename ... Args>
T & Entity::addComponent(Args ...&& args)
{
    Assert(std::is_base_of<Component<T>, T>::value, "");

    auto * ptr = new T(std::forward<Args>(args)...);
    auto uptr = std::unique_ptr<ComponentBase>(ptr);

    m_world.addComponent(m_id, T::index(), std::move(uptr));

    return *ptr;
}

template<typename T>
void Entity::removeComponent()
{
    Assert(std::is_base_of<Component<T>, T>::value, "");

    m_world.removeComponent(m_id, T::index());
}

}

