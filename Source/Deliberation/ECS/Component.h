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
    static constexpr const char * const COMPONENT_NAME = "Unnamed Component";

  public:
    Component();

    virtual std::size_t index() const override;
    virtual std::string name() const override;

  private:
    friend class World;
};
}

#define DELIBERATION_COMPONENT_NAME(name) public: static constexpr const char * const COMPONENT_NAME = name;

#include <Deliberation/ECS/Component.inl>
