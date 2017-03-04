#pragma once

#include <cstdlib>
#include <string>

#include <Deliberation/Core/TypeID.h>

namespace deliberation
{

class AbstractWorld;
class World;

class ComponentBase
{
public:
    virtual ~ComponentBase();

    virtual std::size_t index() const = 0;
    virtual std::string name() const = 0;

    virtual void dispatchEvent(TypeID::value_t eventType, const void * event) = 0;

protected:
    friend class World;

protected:
    AbstractWorld * m_world = nullptr;
    size_t          m_entityIndex = 0;
};

}

