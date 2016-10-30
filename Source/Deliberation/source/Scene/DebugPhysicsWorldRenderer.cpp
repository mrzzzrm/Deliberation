#include <Deliberation/Scene/DebugPhysicsWorldRenderer.h>

#include <iostream>

#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/PhysicsWorld.h>

namespace deliberation
{

DebugPhysicsWorldRenderer::DebugPhysicsWorldRenderer(Context & context,
                                                     const PhysicsWorld & physicsWorld,
                                                     const Camera3D & camera):
    m_context(context),
    m_physicsWorld(physicsWorld),
    m_camera(camera)
{
    m_contactRenderer.reset(m_context, m_camera);
    m_linearVelocityRenderer.reset(m_context, m_camera);
    m_angularVelocityRenderer.reset(m_context, m_camera);
}

void DebugPhysicsWorldRenderer::schedule()
{
    auto & narrowphase = m_physicsWorld.narrowphase();

    /**
     * Count contact points
     */
    auto numContactPoints = 0;
    for (auto & contact : narrowphase.contacts())
    {
        numContactPoints += contact->numPoints();
    }

    /**
     * Allocate enough arrows for contact points
     */
    m_contactRenderer.get().resizeArrows(numContactPoints, {0.2f, 1.0f, 0.2}, true);

    /**
     * Setup arrows
     */
    uint index = 0;
    for (auto & contact : narrowphase.contacts())
    {
        for (uint p = 0; p < contact->numPoints(); p++)
        {
            auto & point = contact->point(p);

            m_contactRenderer.get().arrow(index).reset(point.position, point.normal);

            if (point.velocityBias < 0.01f)
            {
                m_contactRenderer->arrow(index).setColor({0.6f, 0.6f, 0.6f});
            }

            index++;
        }
    }

    /**
     * Linear Velocities and Angular Velocities
     */
    m_linearVelocityRenderer->resizeArrows(m_physicsWorld.rigidBodies().count(), {1.0f, 1.0f, 0.0f}, true);
    m_angularVelocityRenderer->resizeArrows(m_physicsWorld.rigidBodies().count(), {1.0f, 0.0f, 1.0f}, true);

    auto b = 0u;
    for (auto & body : m_physicsWorld.rigidBodies()) {
        auto linearVelocity = body->linearVelocity();
        if (glm::length(linearVelocity) > 0.01f)
        {
            m_linearVelocityRenderer->arrow(b).reset(body->transform().position(), linearVelocity);
        }
        else
        {
            m_linearVelocityRenderer->arrow(b).setVisible(false);
        }

        auto angularVelocity = body->angularVelocity();
        if (glm::length(angularVelocity) > 0.01f)
        {
            m_angularVelocityRenderer->arrow(b).reset(body->transform().position(), angularVelocity);
        }
        else
        {
            m_angularVelocityRenderer->arrow(b).setVisible(false);
        }

        b++;
    }

    /**
     * Commit
     */
    m_linearVelocityRenderer.get().schedule();
    m_angularVelocityRenderer.get().schedule();
    m_contactRenderer.get().schedule();
}

}