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

protected:
    virtual ~AbstractWorld() = default;

protected:
};
}

#include <Deliberation/ECS/AbstractWorld.inl>