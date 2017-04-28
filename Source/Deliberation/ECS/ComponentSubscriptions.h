#pragma once

#include <functional>
#include <unordered_map>
#include <vector>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/ComponentBase.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

struct ComponentSubscriptionsBase
{
    static std::unordered_map<TypeID::value_t, std::vector<TypeID::value_t>> subscriptionsByComponentType;
};

template<typename ComponentType, typename ... Subscriptions>
class ComponentSubscriptions:
    public ComponentSubscriptionsBase
{
public:
    static ComponentSubscriptions<ComponentType, Subscriptions...> & instance();

    void dispatch(ComponentBase & componentBase, TypeID::value_t eventType, const void * event);

private:
    ComponentSubscriptions();

    template<typename Subscription>
    void registerDispatch();

private:
    std::unordered_map<TypeID::value_t,
        std::function<void(ComponentBase & componentBase, const void * event)>> m_dispatcherByEvent;
};

}

#include <Deliberation/ECS/ComponentSubscriptions.inl>