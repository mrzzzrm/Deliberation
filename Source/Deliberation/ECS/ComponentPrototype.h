#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentPrototypeBase.h>

namespace deliberation
{
template<typename ComponentT>
class ComponentPrototype : public ComponentPrototypeBase
{
public:
    virtual void updateComponent(ComponentT & component) = 0;
    bool         hasComponent(const Entity & entity) override;

protected:
    void         onUpdateEntity(Entity entity) override final;
    virtual void initComponent(ComponentT & component) {}
};
}

#include <Deliberation/ECS/ComponentPrototype.inl>