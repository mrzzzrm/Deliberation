#pragma once

#include <Deliberation/ECS/System.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class PhysicsWorldSystem:
    public System<PhysicsWorldSystem>
{
public:
    PhysicsWorldSystem(World & world, PhysicsWorld & physicsWorld):
        Base(world, ComponentFilter::requires<std::shared_ptr<RigidBody>>()),
        m_physicsWorld(physicsWorld)
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        m_physicsWorld.addRigidBody(entity.component<std::shared_ptr<RigidBody>>());
    }

    void onEntityRemoved(Entity & entity) override
    {
        m_physicsWorld.removeRigidBody(entity.component<std::shared_ptr<RigidBody>>());
    }

private:
    PhysicsWorld & m_physicsWorld;
};

}