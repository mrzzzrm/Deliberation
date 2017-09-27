#pragma once

#include <Deliberation/Core/EventListener.h>
#include <Deliberation/ECS/SystemBase.h>
#include <Deliberation/ECS/Activity.h>

namespace deliberation
{
template<typename T>
class System :
    public Activity<T>,
    public SystemBase,
    public EventListener<T>,
    public std::enable_shared_from_this<T>
{
public:
    typedef System<T> Base;

public:
    static std::size_t indexStatic();

public:
    explicit System(World & world, const ComponentFilter & filter = ComponentFilter());

    std::size_t index() const override;
    const char * name() const override { return T::SYSTEM_NAME; }
};
}

#include <Deliberation/ECS/System.inl>
