#pragma once



#include <Deliberation/ECS/SystemBase.h>

namespace deliberation
{

template<typename T>
class System:
    public SystemBase
{
public:
    typedef System<T> Base;

public:
    static std::size_t indexStatic();

public:
    System(World & world, const ComponentFilter & filter = ComponentFilter());

    virtual std::size_t index() const override;
    virtual std::string name() const override;

    template<typename EventType>
    void subscribe();
};

}

#include <Deliberation/ECS/System.inl>

