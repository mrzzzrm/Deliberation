#pragma once

#include <memory>
#include <functional>

#include <Deliberation/Core/EventListenerProxyImpl.h>
#include <Deliberation/Core/TypeID.h>

namespace deliberation
{
class EventDomain;

class EventListenerProxy final:
    public std::enable_shared_from_this<EventListenerProxy>
{
public:
    template<typename EventType, typename ReceiverType>
    static std::shared_ptr<EventListenerProxy> create(ReceiverType & receiver);

public:
    EventListenerProxy(TypeID::value_t eventType, const std::function<void(const void*)> & fn):
        m_proxy(std::make_shared<EventListenerProxyImpl>(eventType, fn))
    {}

    ~EventListenerProxy();

    const std::shared_ptr<EventListenerProxyImpl> & proxy() { return m_proxy; };
    std::shared_ptr<const EventListenerProxyImpl> proxy() const { return m_proxy; };


 private:
    friend class EventDomain;

private:
    std::weak_ptr<EventDomain>          m_domain;
    std::shared_ptr<EventListenerProxyImpl> m_proxy;
};

}

#include <Deliberation/Core/EventListenerProxy.inl>