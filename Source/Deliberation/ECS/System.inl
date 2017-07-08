#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/Core/EventListener.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{
template<typename T>
std::size_t System<T>::indexStatic()
{
    auto i = TypeID::value<SystemBase, T>();
    Assert(i < ECS_MAX_NUM_SYSTEMS, "");

    return i;
}

template<typename T>
System<T>::System(World & world, const ComponentFilter & filter)
    : SystemBase(world, filter)
{
}

template<typename T>
std::size_t System<T>::index() const
{
    return indexStatic();
}

template<typename T>
std::string System<T>::name() const
{
    return typeid(T).name();
}

template<typename T>
template<typename EventType>
void System<T>::subscribeEvent()
{
    m_eventListeners.emplace_back(m_world.events()->subscribe<EventType>(*(T*)this));
}
}
