#pragma once

#include <memory>
#include <functional>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
class EventDomain;

class EventListener final:
    public std::enable_shared_from_this<EventListener>
{
public:
    template<typename EventType, typename ReceiverType>
    static std::shared_ptr<EventListener> create(ReceiverType & receiver);

public:
    EventListener(TypeID::value_t eventType, const std::function<void(const void*)> & fn):
        m_eventType(eventType),
        m_fn(fn)
    {}

    ~EventListener();

    TypeID::value_t eventType() const { return m_eventType; }

    void onEvent(const void * event)
    {
        m_fn(event);
    }

 private:
    friend class EventDomain;

private:
    std::weak_ptr<EventDomain>          m_domain;
    TypeID::value_t                     m_eventType;
    std::function<void(const void*)>    m_fn;
};

}

#include <Deliberation/Core/EventListener.inl>