#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentBase.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class World;

template<typename T>
class Component : public std::enable_shared_from_this<T>, public ComponentBase
{
  public:
    static std::size_t indexStatic();

  public:
    Component();

    virtual std::size_t index() const override;
    virtual std::string name() const override;

  private:
    friend class World;
};
}

#include <Deliberation/ECS/Component.inl>
