#include <type_traits>

#include <Deliberation/ECS/System.h>

namespace deliberation
{

template<typename T, typename ... Args>
T & World::addSystem(Args &&... args)
{
    Assert((std::is_base_of<System<T>, T>::value), "");

    auto system = std::unique_ptr<SystemBase>(new T(std::forward<Args>(args)...));
    m_systems.push_back(std::move(system));
}

}

