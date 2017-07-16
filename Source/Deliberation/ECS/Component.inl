#include <typeinfo>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/AbstractWorld.h>
#include <Deliberation/ECS/Defines.h>

namespace deliberation
{
template<typename T>
Component<T>::Component()
{
};

template<typename T>
std::size_t Component<T>::indexStatic()
{
    auto i = TypeID::value<ComponentBase, T>();
    Assert(i < ECS_MAX_NUM_COMPONENTS, "");

    return i;
}

template<typename T>
std::size_t Component<T>::index() const
{
    return indexStatic();
}

template<typename T>
std::string Component<T>::name() const
{
    return T::COMPONENT_NAME;
}
}
