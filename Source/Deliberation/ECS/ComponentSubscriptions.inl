#include <Deliberation/Core/Swallow.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/ECS/Defines.h>

namespace deliberation
{
template<typename ComponentType, typename... Subscriptions>
ComponentSubscriptions<ComponentType, Subscriptions...> &
ComponentSubscriptions<ComponentType, Subscriptions...>::instance()
{
    static ComponentSubscriptions<ComponentType, Subscriptions...> instance;
    return instance;
}

template<typename ComponentType, typename... Subscriptions>
ComponentSubscriptions<ComponentType, Subscriptions...>::
    ComponentSubscriptions()
{
    Swallow((registerDispatch<Subscriptions>(), 0)...);
}

template<typename ComponentType, typename... Subscriptions>
void ComponentSubscriptions<ComponentType, Subscriptions...>::dispatch(
    ComponentBase & componentBase,
    TypeID::value_t eventType,
    const void *    event)
{
    auto it = m_dispatcherByEvent.find(eventType);
    Assert(it != m_dispatcherByEvent.end(), "");

    it->second(componentBase, event);
}

template<typename ComponentType, typename... Subscriptions>
template<typename Subscription>
void ComponentSubscriptions<ComponentType, Subscriptions...>::registerDispatch()
{
    const auto eventType = TypeID::value<ComponentEventFamily, Subscription>();

    subscriptionsByComponentType[ComponentType::indexStatic()].emplace_back(
        eventType);

    m_dispatcherByEvent[eventType] = [](ComponentBase & componentBase,
                                        const void *    event) {
        auto & component = *static_cast<ComponentType *>(&componentBase);
        component.receive(*(const Subscription *)event);
    };
};
}