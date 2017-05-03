#include <Deliberation/Scene/Debug/DebugPhysicsWorldRenderer.h>

#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/PhysicsWorld.h>

namespace deliberation
{

DebugPhysicsWorldRenderer::DebugPhysicsWorldRenderer(DrawContext & drawContext,
                                                     const PhysicsWorld & physicsWorld,
                                                     const Camera3D & camera):
    m_drawContext(drawContext),
    m_physicsWorld(physicsWorld),
    m_camera(camera)
{
//    m_contactNormalRenderer.reset(m_drawContext, m_camera);
//    m_contactVelocityRenderer.reset(m_drawContext, m_camera);
//    m_linearVelocityRenderer.reset(m_drawContext, m_camera);
//    m_angularVelocityRenderer.reset(m_drawContext, m_camera);
}

void DebugPhysicsWorldRenderer::render()
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
//    m_contactNormalRenderer->resizeArrows(numContactPoints/*, {0.2f, 1.0f, 0.2}, true*/);
//    m_contactVelocityRenderer->resizeArrows(numContactPoints * 2/*, {0.2f, 0.7f, 0.4}, true*/);

    /**
     * Setup contacts
     */
    uint index = 0;
    for (auto & contact : narrowphase.contacts())
    {
        for (uint p = 0; p < contact->numPoints(); p++)
        {
            auto & point = contact->point(p);
            auto & bodyA = contact->bodyA();
            auto & bodyB = contact->bodyB();

//            {
//                auto &arrow = m_contactNormalRenderer->arrow(index);
//                arrow.setOrigin(point.position);
//                arrow.setDelta(point.normal);
//            }

            auto localVA = bodyA.localVelocity(point.position - bodyA.transform().position());
            auto localVB = bodyB.localVelocity(point.position - bodyB.transform().position());

//            m_contactVelocityRenderer->arrow(2 * index + 0).reset(point.position, localVA);
//            m_contactVelocityRenderer->arrow(2 * index + 1).reset(point.position, localVB);

            if (point.velocityBias < 0.01f)
            {
                // Desaturate inactive contacts
                m_contactNormalRenderer->arrow(index).setColor({0.6f, 0.6f, 0.6f});
            }

            index++;
        }
    }

    /**
     * Linear Velocities and Angular Velocities
     */
//    m_linearVelocityRenderer->resizeArrows(m_physicsWorld.rigidBodies().count(), {1.0f, 1.0f, 0.0f}, true);
//    m_angularVelocityRenderer->resizeArrows(m_physicsWorld.rigidBodies().count(), {1.0f, 0.0f, 1.0f}, true);

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
//    m_linearVelocityRenderer->render();
//    m_angularVelocityRenderer->render();
//    m_contactNormalRenderer->render();
//    m_contactVelocityRenderer->render();
}

}