#pragma once

#include <memory>
#include <vector>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/EventListenerProxy.h>

namespace deliberation
{
template<typename T>
class EventListener
{
public:
    EventListener(const std::shared_ptr<EventDomain> & events)
        : m_events(events)
    {
    }

    template<typename EventType>
    void subscribeEvent()
    {
        auto events = m_events.lock();
        Assert(static_cast<bool>(events), "");

        m_eventListenerProxies.emplace_back(
            events->template subscribe<EventType>(*(T *)this));
    }

private:
    std::weak_ptr<EventDomain>                       m_events;
    std::vector<std::shared_ptr<EventListenerProxy>> m_eventListenerProxies;
};
}