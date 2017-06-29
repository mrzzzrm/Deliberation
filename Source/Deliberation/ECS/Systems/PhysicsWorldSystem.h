#pragma once

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class PhysicsWorldSystem : public System<PhysicsWorldSystem>
{
  public:
    PhysicsWorldSystem(World & world, PhysicsWorld & physicsWorld)
        : Base(world, ComponentFilter::requires<RigidBodyComponent>())
        , m_physicsWorld(physicsWorld)
    {
    }

    PhysicsWorld & physicsWorld() const { return m_physicsWorld; }

    void updatePhysics(float seconds)
    {
        m_physicsWorld.update(seconds);

        for (auto & entityEntry : m_entities)
        {
            if (!entityEntry.active) continue;

            auto entity = m_world.entity(entityEntry.id);
            if (entity.hasComponent<Transform3DComponent>())
            {
                entity.component<Transform3DComponent>().value() =
                    entity.component<RigidBodyComponent>().value()->transform();
            }
        }
    }

  protected:
    void onEntityAdded(Entity & entity) override
    {
        auto rigidBody = entity.component<RigidBodyComponent>().value();
        rigidBody->setEntity(entity);

        m_physicsWorld.addRigidBody(rigidBody);
    }

    void onEntityRemoved(Entity & entity) override
    {
        m_physicsWorld.removeRigidBody(
            entity.component<RigidBodyComponent>().value());
    }

    void onEntityPrePhysicsUpdate(Entity & entity, float physicsTimestep) override
    {
        if (entity.hasComponent<Transform3DComponent>())
        {
            entity.component<RigidBodyComponent>().value()->transform() =
                entity.component<Transform3DComponent>().value();
        }
    }

  private:
    PhysicsWorld & m_physicsWorld;
};
}