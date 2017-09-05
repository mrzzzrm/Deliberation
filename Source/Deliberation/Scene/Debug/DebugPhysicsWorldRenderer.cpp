#include <Deliberation/Scene/Debug/DebugPhysicsWorldRenderer.h>

#include <iostream>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/PrimitiveTester.h>
#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Pipeline/RenderSystem.h>

namespace deliberation
{
DebugPhysicsWorldRenderer::DebugPhysicsWorldRenderer(
    RenderManager & renderManager,
    RenderSystem &  renderSystem,
    PhysicsWorld &  phyicsWorld)
    : SingleNodeRenderer(renderManager, RenderPhase::PreOverlay)
    , m_physicsWorld(phyicsWorld)
{
    m_debugNode = renderSystem.debugGeometryRenderer()->addNode();
}

void DebugPhysicsWorldRenderer::render()
{
    size_t boxIndex = 0;
    for (const auto & rigidBody : m_physicsWorld.rigidBodies())
    {
        auto aabb = rigidBody->bounds();

        auto & box = m_debugNode->box(boxIndex);
        box.setHalfExtent(glm::vec3(aabb.urb() - aabb.llf()) / 2.0f);
        box.setTransform(Transform3D::atPosition(aabb.center()));
        box.setVisible(true);
        box.setWireframe(true);
        box.setColor(glm::vec3(0.2, 1.0f, 0.2f));

        boxIndex++;
    }

    for (; boxIndex < m_numAllocatedBoxes; boxIndex++)
    {
        m_debugNode->box(boxIndex).setVisible(false);
    }
    m_numAllocatedBoxes = boxIndex;
}
}