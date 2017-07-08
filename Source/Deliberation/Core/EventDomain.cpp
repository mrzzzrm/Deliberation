#include <Deliberation/Core/EventDomain.h>

namespace deliberation
{

void EventDomain::addEventListener(const std::shared_ptr<EventListener> & listener)
{
    m_eventListenersByEventType[listener->eventType()].emplace_back(listener);
    listener->m_domain = shared_from_this();
}

void EventDomain::removeEventListener(const std::shared_ptr<EventListener> & listener)
{
    auto & listeners = m_eventListenersByEventType[listener->eventType()];
    auto iter = std::find_if(listeners.begin(), listeners.end(), [&] (auto & listener2)
    {
      return listener2.lock() == listener;
    });
    Assert(iter != listeners.end(), "");
    listeners.erase(iter);
}

}
