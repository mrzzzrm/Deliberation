#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
template<typename EventType, typename ReceiverType>
std::shared_ptr<EventListener> EventDomain::subscribe(ReceiverType & receiver)
{
    auto listener = EventListener::create<EventType>(receiver);
    addEventListener(listener);
    return listener;
}

template<typename EventType>
void EventDomain::publishEvent(const EventType & event)
{
    auto eventTypeId = TypeID::value<EventListener, EventType>();
    auto & listenerProxies = m_eventListenerProxiesByEventType[eventTypeId];
    
    for (auto & listenerProxy : listenerProxies)
    {
        listenerProxy->onEvent(&event);
    }
}
}
