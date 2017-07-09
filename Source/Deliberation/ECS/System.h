#pragma once

#include <Deliberation/Core/EventListener.h>
#include <Deliberation/ECS/SystemBase.h>

namespace deliberation
{
template<typename T>
class System : public SystemBase, public EventListener<T>
{
  public:
    typedef System<T> Base;

  public:
    static std::size_t indexStatic();

  public:
    System(World & world, const ComponentFilter & filter = ComponentFilter());

    virtual std::size_t index() const override;
    virtual std::string name() const override;

};
}

#include <Deliberation/ECS/System.inl>
