#pragma once

#include <memory>

#include <Deliberation/Core/EventDomain.h>
#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class EventListenerProxy;
class SystemBase;

class AbstractWorld
{
  public:
    AbstractWorld();

    std::shared_ptr<EventDomain> events() { return m_events; }
    const std::shared_ptr<EventDomain> & events() const { return m_events; }

  protected:
    virtual ~AbstractWorld() = default;

protected:
    std::shared_ptr<EventDomain> m_events;
};
}

#include <Deliberation/ECS/AbstractWorld.inl>