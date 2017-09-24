#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>
#include <Deliberation/Core/EventListenerProxy.h>

#include <Deliberation/ECS/World.h>

#include <Deliberation/Platform/App.h>
#include <Deliberation/Platform/AppRuntime.h>

namespace deliberation
{
template<typename T>
std::size_t System<T>::indexStatic()
{
    auto i = TypeID::value<SystemBase, T>();
    Assert(i < ECS_MAX_NUM_SYSTEMS);

    return i;
}

template<typename T>
System<T>::System(World & world, const ComponentFilter & filter)
    : SystemBase(world, filter), EventListener<T>(App::get().runtime()->events())
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
}
