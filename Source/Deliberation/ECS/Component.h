#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentBase.h>
#include <Deliberation/ECS/ComponentSubscriptions.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class World;

template<
    typename T,
    typename ComponentSubscriptionsType = ComponentSubscriptions<T>>
class Component : public std::enable_shared_from_this<T>, public ComponentBase
{
  public:
    static std::size_t indexStatic();

  public:
    Component();

    virtual std::size_t index() const override;
    virtual std::string name() const override;

    template<typename Event>
    void emit(const Event & event);

    void
    dispatchEvent(TypeID::value_t eventType, const void * event) final override;

  private:
    friend class World;
};
}

#include <Deliberation/ECS/Component.inl>
