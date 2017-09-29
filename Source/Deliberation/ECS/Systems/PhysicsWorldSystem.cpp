#include <Deliberation/ECS/Systems/PhysicsWorldSystem.h>

#include <Deliberation/Core/UpdateFrame.h>

#include <Deliberation/ECS/RigidBodyComponent.h>
#include <Deliberation/ECS/System.h>
#include <Deliberation/ECS/Transform3DComponent.h>
#include <Deliberation/ECS/World.h>

#include <Deliberation/Physics/PhysicsWorld.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
PhysicsWorldSystem::PhysicsWorldSystem(World & world)
    : Base(world, ComponentFilter::requires<RigidBodyComponent, Transform3DComponent>())
{
}

float PhysicsWorldSystem::updatePhysics(const UpdateFrame & updateFrame)
{
    return m_physicsWorld.update(updateFrame.gameSeconds());
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

    auto & transformComponent = entity.component<Transform3DComponent>();
    transformComponent.referenceBody(rigidBody);

    m_physicsWorld.addRigidBody(rigidBody);
}

void PhysicsWorldSystem::onEntityRemoved(Entity & entity)
{
    m_physicsWorld.removeRigidBody(
        entity.component<RigidBodyComponent>().value());
}

}