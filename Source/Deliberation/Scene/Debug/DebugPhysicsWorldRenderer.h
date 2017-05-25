#pragma once

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>

namespace deliberation
{
class Camera3D;
class DrawContext;
class PhysicsWorld;

class DebugPhysicsWorldRenderer final
{
  public:
    DebugPhysicsWorldRenderer(
        DrawContext &        drawContext,
        const PhysicsWorld & physicsWorld,
        const Camera3D &     camera);

    void render();

  private:
    DrawContext &        m_drawContext;
    const PhysicsWorld & m_physicsWorld;
    const Camera3D &     m_camera;

    Optional<DebugGeometryNode> m_contactNormalRenderer;
    Optional<DebugGeometryNode> m_contactVelocityRenderer;
    Optional<DebugGeometryNode> m_linearVelocityRenderer;
    Optional<DebugGeometryNode> m_angularVelocityRenderer;
};
}