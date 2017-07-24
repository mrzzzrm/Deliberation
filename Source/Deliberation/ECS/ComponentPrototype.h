#pragma once

#include <memory>

#include <Deliberation/ECS/ComponentPrototypeBase.h>
#include <Deliberation/ECS/World.h>

namespace deliberation
{
template<typename ComponentT>
class ComponentPrototype : public ComponentPrototypeBase
{
public:
    World & world() const { Assert(m_world != nullptr, "World not yet set"); return *m_world; }
    void setWorld(World & world) { m_world = &world; }

    virtual void updateComponent(ComponentT & component) = 0;
    bool         hasComponent(const Entity & entity) override;

protected:
    void         onUpdateEntity(Entity entity) final;
    virtual void initComponent(ComponentT & component) {}

protected:
    World * m_world = nullptr;
};
}

#include <Deliberation/ECS/ComponentPrototype.inl>