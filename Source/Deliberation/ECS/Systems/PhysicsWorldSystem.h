#pragma once

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Scene/Debug/DebugPhysicsWorldRenderer.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class PhysicsWorldSystem : public System<PhysicsWorldSystem>
{
  public:
    PhysicsWorldSystem(World & world, PhysicsWorld & physicsWorld);

    PhysicsWorld & physicsWorld() const { return m_physicsWorld; }

    void updatePhysics(float seconds);

    void onCreated() override;
    void onRemoved() override;

  protected:
    void onEntityAdded(Entity & entity) override;
    void onEntityRemoved(Entity & entity) override;
    void onEntityPrePhysicsUpdate(Entity & entity, float physicsTimestep) override;

  private:
    PhysicsWorld & m_physicsWorld;
    std::shared_ptr<DebugPhysicsWorldRenderer> m_debugRenderer;
};
}