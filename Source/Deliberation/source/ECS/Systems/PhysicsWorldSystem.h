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

    PhysicsWorld & physicsWorld() const { return m_physicsWorld; }

protected:
    void onEntityAdded(Entity & entity) override
    {
        auto rigidBody = entity.component<RigidBodyComponent>().value();
        rigidBody->setEntity(entity);

        m_physicsWorld.addRigidBody(rigidBody);
    }

    void onEntityRemoved(Entity & entity) override
    {
        m_physicsWorld.removeRigidBody(entity.component<RigidBodyComponent>().value());
    }

private:
    PhysicsWorld & m_physicsWorld;
};

}