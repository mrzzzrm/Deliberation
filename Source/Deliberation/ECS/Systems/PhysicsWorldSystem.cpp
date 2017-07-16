#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>

#include <Deliberation/ECS/Components.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Scene/Pipeline/RenderSystem.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
PhysicsWorldSystem::PhysicsWorldSystem(
    World & world, PhysicsWorld & physicsWorld)
    : Base(world, ComponentFilter::requires<RigidBodyComponent>())
    , m_physicsWorld(physicsWorld)
{
}

void PhysicsWorldSystem::updatePhysics(float seconds)
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

void PhysicsWorldSystem::onCreated()
{
    //    auto renderSystem = world().system<RenderSystem>();
    //
    //    if (renderSystem)
    //    {
    //        m_debugRenderer =
    //            renderSystem->renderManager().addRenderer<DebugPhysicsWorldRenderer>(*renderSystem,
    //            m_physicsWorld);
    //    }
    //    else
    //    {
    //        std::cout << "PhysicsWorldSystem: No RenderSystem available, not
    //        adding DebugPhysicsWorldRenderer" << std::endl;
    //    }
}

void PhysicsWorldSystem::onRemoved()
{
    //    auto renderSystem = world().system<RenderSystem>();
    //    if (renderSystem)
    //    {
    //        // TODO add once this is possible/makes sense
    //        // renderSystem->renderManager().removeRenderer(m_debugRenderer);
    //    }
}

void PhysicsWorldSystem::onEntityAdded(Entity & entity)
{
    auto & rigidBody = entity.component<RigidBodyComponent>().value();
    rigidBody->setEntity(entity);

    m_physicsWorld.addRigidBody(rigidBody);
}

void PhysicsWorldSystem::onEntityRemoved(Entity & entity)
{
    m_physicsWorld.removeRigidBody(
        entity.component<RigidBodyComponent>().value());
}

void PhysicsWorldSystem::onEntityPrePhysicsUpdate(
    Entity & entity, float physicsTimestep)
{
    if (entity.hasComponent<Transform3DComponent>())
    {
        entity.component<RigidBodyComponent>().value()->transform() =
            entity.component<Transform3DComponent>().value();
    }
}
}