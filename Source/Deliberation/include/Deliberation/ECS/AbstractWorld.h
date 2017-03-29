#pragma once

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class AbstractWorld
{
public:
    virtual void emit(size_t entityIndex, TypeID::value_t eventType, const void * event) = 0;

protected:
    virtual ~AbstractWorld() = default;
};

}