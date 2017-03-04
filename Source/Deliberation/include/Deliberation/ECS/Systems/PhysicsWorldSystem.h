#pragma once

#include <Deliberation/ECS/Components.h>
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
        Base(world, ComponentFilter::requires<RigidBodyComponent>()),
        m_physicsWorld(physicsWorld)
    {}

protected:
    void onEntityAdded(Entity & entity) override
    {
        m_physicsWorld.addRigidBody(entity.component<RigidBodyComponent>().value());
    }

    void onEntityRemoved(Entity & entity) override
    {
        m_physicsWorld.removeRigidBody(entity.component<RigidBodyComponent>().value());
    }

private:
    PhysicsWorld & m_physicsWorld;
};

}