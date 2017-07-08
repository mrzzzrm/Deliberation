#pragma once

#include <memory>
#include <unordered_map>

#include <Deliberation/Core/SparseVector.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/Core/EventListener.h>

namespace deliberation
{
class EventDomain final:
    std::enable_shared_from_this<EventDomain>
{
  public:
    template<typename EventType, typename ReceiverType>
    std::shared_ptr<EventListener> subscribe(ReceiverType & receiver);

    void addEventListener(const std::shared_ptr<EventListener> & listener);
    void removeEventListener(const std::shared_ptr<EventListener> & listener);

    template<typename EventType>
    void publishEvent(const EventType & event);

  private:
    std::unordered_map<TypeID::value_t, std::vector<std::weak_ptr<EventListener>>>
        m_eventListenersByEventType;
};
}

#include <Deliberation/Core/EventDomain.inl>
