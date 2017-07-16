#pragma once

#include <Deliberation/ECS/Component.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
template<typename WrappedType, typename ComponentType>
class ComponentWrapper : public Component<
                                   ComponentWrapper<WrappedType, ComponentType>>
{
DELIBERATION_COMPONENT_NAME("Unnamed ComponentWrapper")
public:
    typedef ComponentWrapper<WrappedType, ComponentType> Base;
  public:
    template<typename... Args>
    ComponentWrapper(Args &&... args);

    std::string name() const override { return ComponentType::COMPONENT_NAME; }

    WrappedType &       value();
    const WrappedType & value() const;

  private:
    WrappedType m_value;
};
}

#include <Deliberation/ECS/ComponentWrapper.inl>