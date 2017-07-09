#pragma once

#include <memory>
#include <functional>

#include <Deliberation/Core/EventListenerProxy.h>
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
        m_proxy(std::make_shared<EventListenerProxy>(eventType, fn))
    {}

    ~EventListener();

    const std::shared_ptr<EventListenerProxy> & proxy() { return m_proxy; };
    std::shared_ptr<const EventListenerProxy> proxy() const { return m_proxy; };


 private:
    friend class EventDomain;

private:
    std::weak_ptr<EventDomain>          m_domain;
    std::shared_ptr<EventListenerProxy> m_proxy;
};

}

#include <Deliberation/Core/EventListener.inl>