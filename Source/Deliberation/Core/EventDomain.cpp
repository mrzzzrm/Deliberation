#include <Deliberation/Core/EventDomain.h>

namespace deliberation
{

void EventDomain::addEventListener(const std::shared_ptr<EventListener> &listener)
{
    m_eventListenerProxiesByEventType[listener->proxy()->eventType()].emplace_back(listener->proxy());
    listener->m_domain = shared_from_this();
}

void EventDomain::removeEventListener(const EventListener &listener)
{
    auto & listenerProxies = m_eventListenerProxiesByEventType[listener.proxy()->eventType()];
    auto iter = std::find_if(listenerProxies.begin(), listenerProxies.end(), [&] (auto & listenerProxy)
    {
      return listenerProxy == listener.proxy();
    });
    Assert(iter != listenerProxies.end(), "");
    listenerProxies.erase(iter);
}

}
