#pragma once

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Program.h>

#include <Deliberation/Scene/Pipeline/RenderManager.h>
#include <Deliberation/Scene/Pipeline/Renderer.h>

namespace deliberation
{
class Camera3D;
class DrawContext;
class Framebuffer;

class DebugGroundPlaneRenderer : public Renderer
{
public:
    DebugGroundPlaneRenderer(RenderManager & renderManager);

    void setSize(float size);
    void setQuadSize(float quadSize);
    void setRadius(float radius);
    void setRenderToGBuffer(bool enabled) { m_renderToGBuffer = enabled; }

    void registerRenderNodes() override;

private:
    friend class DebugGroundPlaneNode;

private:
    float m_size = 3.0f;
    float m_quadSize = 0.5f;
    float m_radius = 3.0f;
    bool  m_renderToGBuffer = false;
};
}
