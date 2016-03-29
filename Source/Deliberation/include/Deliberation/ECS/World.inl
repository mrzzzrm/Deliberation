#include <type_traits>

#include <Deliberation/ECS/System.h>

namespace deliberation
{

template<typename T, typename ... Args>
T & World::addSystem(Args &&... args)
{
    Assert((std::is_base_of<System<T>, T>::value), "");

    auto * ptr = new T(*this, std::forward<Args>(args)...);
    auto system = std::unique_ptr<SystemBase>(ptr);
    m_systems.emplace(system->index(), std::move(system));

    return * ptr;
}

}

