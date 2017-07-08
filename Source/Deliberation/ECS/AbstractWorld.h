#pragma once

#include <memory>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class EventListener;
class SystemBase;

class AbstractWorld
{
  public:
    virtual void
    publishEvent(TypeID::value_t eventType, const void * event) = 0;

    virtual void addEventListener(const std::shared_ptr<EventListener> & listener) = 0;
    virtual void removeEventListener(const std::shared_ptr<EventListener> & listener) = 0;

  protected:
    virtual ~AbstractWorld() = default;
};
}

#include <Deliberation/ECS/AbstractWorld.inl>