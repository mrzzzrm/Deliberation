#include <typeinfo>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/AbstractWorld.h>
#include <Deliberation/ECS/Defines.h>

namespace deliberation
{
template<typename T, typename ComponentSubscriptionsType>
Component<T, ComponentSubscriptionsType>::Component()
{
    ComponentSubscriptionsType::instance();
};

template<typename T, typename ComponentSubscriptionsType>
std::size_t Component<T, ComponentSubscriptionsType>::indexStatic()
{
    auto i = TypeID::value<ComponentBase, T>();
    Assert(i < ECS_MAX_NUM_COMPONENTS, "");

    return i;
}

template<typename T, typename ComponentSubscriptionsType>
std::size_t Component<T, ComponentSubscriptionsType>::index() const
{
    return indexStatic();
}

template<typename T, typename ComponentSubscriptionsType>
std::string Component<T, ComponentSubscriptionsType>::name() const
{
    return typeid(T).name();
}

template<typename T, typename ComponentSubscriptionsType>
template<typename Event>
void Component<T, ComponentSubscriptionsType>::emit(const Event & event)
{
    Assert(m_world, "") m_world->emitEvent(
        m_entityIndex, TypeID::value<ComponentEventFamily, Event>(), &event);
}

template<typename T, typename ComponentSubscriptionsType>
void Component<T, ComponentSubscriptionsType>::dispatchEvent(
    TypeID::value_t eventType, const void * event)
{
    ComponentSubscriptionsType::instance().dispatch(*this, eventType, event);
}
}
