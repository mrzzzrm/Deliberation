#include <type_traits>

#include <Deliberation/ECS/System.h>

namespace deliberation
{
template<typename T>
std::shared_ptr<T> World::system()
{
    Assert((std::is_base_of<SystemBase, T>::value), "");

    const auto index = System<T>::indexStatic();

    if (!m_systems.contains(index)) return {};

    auto systemBase = m_systems.at(index);
    return std::static_pointer_cast<T>(systemBase);
}

template<typename T>
T & World::systemRef()
{
    auto systemPtr = system<T>();
    Assert((bool)systemPtr, "No such system");
    return *systemPtr;
}

template<typename T, typename... Args>
std::shared_ptr<T> World::addSystem(Args &&... args)
{
    Assert((std::is_base_of<SystemBase, T>::value), "");

    auto system = std::make_shared<T>(*this, std::forward<Args>(args)...);
    m_systems.emplace(system->index(), system);

    system->onCreated();

    return system;
}
}
