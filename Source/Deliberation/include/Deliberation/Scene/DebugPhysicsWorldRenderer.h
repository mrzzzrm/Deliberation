#pragma once

#include <Deliberation/Core/Optional.h>

#include <Deliberation/Scene/DebugGeometryRenderer.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Camera3D;
class Context;
class PhysicsWorld;

class DELIBERATION_API DebugPhysicsWorldRenderer final
{
public:
    DebugPhysicsWorldRenderer(Context & context,
                              const PhysicsWorld & physicsWorld,
                              const Camera3D & camera);

    void schedule();

private:
    Context &                       m_context;
    const PhysicsWorld &            m_physicsWorld;
    const Camera3D &                m_camera;

    Optional<DebugGeometryRenderer> m_contactRenderer;
    Optional<DebugGeometryRenderer> m_linearVelocityRenderer;
    Optional<DebugGeometryRenderer> m_angularVelocityRenderer;
};

}