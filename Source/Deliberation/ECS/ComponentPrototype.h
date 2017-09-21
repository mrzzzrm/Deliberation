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
    std::shared_ptr<World> world() const { return m_world.lock(); }
    void setWorld(const std::shared_ptr<World> & world) { m_world = world; }

    virtual void updateComponent(const Entity & entity, ComponentT & component) = 0;
    bool         hasComponent(const Entity & entity) override;

protected:
    void         onUpdateEntity(Entity entity) final;
    virtual void initComponent(const Entity & entity, ComponentT & component) {}

protected:
    std::weak_ptr<World> m_world;
};
}

#include <Deliberation/ECS/ComponentPrototype.inl>