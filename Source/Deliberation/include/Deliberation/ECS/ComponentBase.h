#pragma once

#include <cstdlib>
#include <string>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class AbstractWorld;
class World;

enum class ComponentPhase
{
    Active,
    ScheduledForRemoval
};

class ComponentBase
{
public:
    virtual ~ComponentBase();

    virtual std::size_t index() const = 0;
    virtual std::string name() const = 0;

    ComponentPhase phase() const { return m_phase; }

    void setPhase(ComponentPhase phase) { m_phase = phase; }

    virtual void dispatchEvent(TypeID::value_t eventType, const void * event) = 0;

protected:
    friend class World;

protected:
    AbstractWorld * m_world = nullptr;
    size_t          m_entityIndex = 0;
    ComponentPhase  m_phase = ComponentPhase::Active;
};

}

