#pragma once

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>



namespace deliberation
{

class Camera3D;
class DrawContext;
class PhysicsWorld;

class DebugPhysicsWorldRenderer final
{
public:
    DebugPhysicsWorldRenderer(DrawContext & drawContext,
                              const PhysicsWorld & physicsWorld,
                              const Camera3D & camera);

    void schedule();

private:
    DrawContext &                       m_drawContext;
    const PhysicsWorld &            m_physicsWorld;
    const Camera3D &                m_camera;

    Optional<DebugGeometryRenderer> m_contactNormalRenderer;
    Optional<DebugGeometryRenderer> m_contactVelocityRenderer;
    Optional<DebugGeometryRenderer> m_linearVelocityRenderer;
    Optional<DebugGeometryRenderer> m_angularVelocityRenderer;
};

}