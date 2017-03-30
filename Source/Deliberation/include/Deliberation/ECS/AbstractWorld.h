#pragma once

#include <memory>

#include <Deliberation/Core/TypeID.h>

#include <Deliberation/ECS/Defines.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class SystemBase;

class AbstractWorld
{
public:
    virtual void emit(size_t entityIndex, TypeID::value_t eventType, const void * event) = 0;

    template<typename T>
    T & system();

    virtual std::shared_ptr<SystemBase> system(SystemTypeId systemTypeId) = 0;

protected:
    virtual ~AbstractWorld() = default;

};

}

#include <Deliberation/ECS/AbstractWorld.inl>