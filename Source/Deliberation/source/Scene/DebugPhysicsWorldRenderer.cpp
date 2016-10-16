#include <Deliberation/Scene/DebugPhysicsWorldRenderer.h>

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
}

void DebugPhysicsWorldRenderer::schedule()
{
    auto & narrowphase = m_physicsWorld.narrowphase();

    auto numContactPoints = 0;
    for (auto & contact : narrowphase.contacts())
    {
        numContactPoints += contact->numPoints();
    }

    m_contactRenderer.get().resizeArrows(numContactPoints, {0.2f, 1.0f, 0.2}, true);

    uint index = 0;
    for (auto & contact : narrowphase.contacts())
    {
        for (uint p = 0; p < contact->numPoints(); p++)
        {
            auto & point = contact->point(p);
            m_contactRenderer.get().arrow(index).reset(point.position, point.normal);
            index++;
        }
    }

    m_contactRenderer.get().schedule();
}

}