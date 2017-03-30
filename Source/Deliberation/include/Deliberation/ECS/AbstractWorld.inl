#include <Deliberation/ECS/AbstractWorld.h>

#include <type_traits>

namespace deliberation
{

template<typename T>
T & World::system()
{
    Assert((std::is_base_of<System<T>, T>::value), "");

    auto systemBase = m_systems.at(System<T>::indexStatic());
    return *std::static_pointer_cast<T>(systemBase);
}

}
