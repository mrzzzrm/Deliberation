#include <type_traits>

#include <Deliberation/ECS/System.h>

namespace deliberation
{

template<typename T>
T & World::system()
{
    Assert((std::is_base_of<System<T>, T>::value), "");

    auto systemBase = m_systems.at(System<T>::indexStatic());
    return *std::static_pointer_cast<T>(systemBase);
}


template<typename T, typename ... Args>
std::shared_ptr<T> World::addSystem(Args &&... args)
{
    Assert((std::is_base_of<SystemBase, T>::value), "");

    auto system = std::make_shared<T>(*this, std::forward<Args>(args)...);
    m_systems.emplace(system->index(), system);

    system->onCreated();

    return system;
}

}

