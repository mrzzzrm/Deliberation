#pragma once

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>
#include <Deliberation/Scene/Pipeline/SingleNodeRenderer.h>

namespace deliberation
{
class Camera3D;
class PhysicsWorld;

class DebugPhysicsWorldRenderer : public SingleNodeRenderer
{
public:
    DebugPhysicsWorldRenderer(
        PhysicsWorld &  phyicsWorld);

    void render() override;

private:
    PhysicsWorld &                     m_physicsWorld;
    std::shared_ptr<DebugGeometryNode> m_debugNode;
    size_t                             m_numAllocatedBoxes = 0;
};
}