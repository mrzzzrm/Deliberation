#pragma once

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class EventListenerProxy
{
public:
    EventListenerProxy(TypeID::value_t eventType, const std::function<void(const void*)> & fn):
        m_eventType(eventType),
        m_fn(fn)
    {}

    TypeID::value_t eventType() const { return m_eventType; }

    void onEvent(const void * event)
    {
        m_fn(event);
    }

private:
    friend class EventListener;

private:
    TypeID::value_t                     m_eventType;
    std::function<void(const void*)>    m_fn;
};

}