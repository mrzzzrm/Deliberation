#pragma once

#include <memory>
#include <unordered_map>

#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/Core/EventListenerProxy.h>

namespace deliberation
{
class EventDomain final : public std::enable_shared_from_this<EventDomain>
{
public:
    template<typename EventType, typename ReceiverType>
    std::shared_ptr<EventListenerProxy> subscribe(ReceiverType & receiver);

    void addEventListener(const std::shared_ptr<EventListenerProxy> & listener);
    void removeEventListener(const EventListenerProxy & listener);

    template<typename EventType>
    void publishEvent(const EventType & event);

private:
    std::unordered_map<
        TypeID::value_t,
        std::vector<std::shared_ptr<EventListenerProxyImpl>>>
        m_eventListenerProxiesByEventType;
};
}

#include <Deliberation/Core/EventDomain.inl>
