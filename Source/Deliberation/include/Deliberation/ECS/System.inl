#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

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
System<T>::System(World & world, const ComponentFilter & filter):
    SystemBase(world, filter)
{

}

template<typename T>
std::size_t System<T>::index() const
{
    return indexStatic();
}

}

