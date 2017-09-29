#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/Pipeline/RenderNode.h>

namespace deliberation
{
class DebugGroundPlaneNode : public RenderNode
{
public:
    DebugGroundPlaneNode(
        DebugGroundPlaneRenderer &       renderer,
        const std::vector<std::string> & shaders)
        : RenderNode(renderer.shared_from_this()), m_renderer(renderer)
    {
        m_program = GetGlobal<DrawContext>()->createProgram(shaders);
        m_draw =
            GetGlobal<DrawContext>()->createDraw(m_program, DrawPrimitive::TriangleStrip);

        m_view = m_draw.uniform("View");
        m_projection = m_draw.uniform("Projection");

        m_size = m_draw.uniform("Size");
        m_quadSize = m_draw.uniform("QuadSize");
        m_radius = m_draw.uniform("Radius");

        LayoutedBlob vertices({"Position", Type_Vec3}, 4);

        auto positions = vertices.field<glm::vec3>("Position");

        positions[0] = {0.5f, 0.0f, 0.5f};
        positions[1] = {0.5, 0.0f, -0.5f};
        positions[2] = {-0.5f, 0.0f, 0.5f};
        positions[3] = {-0.5f, 0.0f, -0.5f};

        m_draw.state().setCullState(CullState::disabled());
        m_draw.addVertices(vertices);
    }

    void render() override
    {
        m_size.set(m_renderer.m_size);
        m_quadSize.set(m_renderer.m_quadSize);
        m_radius.set(m_renderer.m_radius);

        m_view.set(GetGlobal<RenderManager>()->mainCamera().view());
        m_projection.set(GetGlobal<RenderManager>()->mainCamera().projection());

        m_draw.render();
    }

protected:
    DebugGroundPlaneRenderer & m_renderer;
    Program                    m_program;
    Draw                       m_draw;
    Uniform                    m_view;
    Uniform                    m_projection;
    Uniform                    m_size;
    Uniform                    m_quadSize;
    Uniform                    m_radius;
};

class DebugGroundPlaneForwardNode : public DebugGroundPlaneNode
{
public:
    DebugGroundPlaneForwardNode(DebugGroundPlaneRenderer & renderer)
        : DebugGroundPlaneNode(
              renderer,
              {DeliberationDataPath(
                   "Data/Shaders/DebugGroundPlaneRenderer.vert"),
               DeliberationDataPath(
                   "Data/Shaders/DebugGroundPlaneRendererForward.frag")})
    {
    }
};

class DebugGroundPlaneGBufferNode : public DebugGroundPlaneNode
{
public:
    DebugGroundPlaneGBufferNode(DebugGroundPlaneRenderer & renderer)
        : DebugGroundPlaneNode(
              renderer,
              {DeliberationDataPath(
                   "Data/Shaders/DebugGroundPlaneRenderer.vert"),
               DeliberationDataPath(
                   "Data/Shaders/DebugGroundPlaneRendererGBuffer.frag")})
    {
        m_draw.setFramebuffer(GetGlobal<RenderManager>()->gbuffer());
    }
};

DebugGroundPlaneRenderer::DebugGroundPlaneRenderer()
{
}

void DebugGroundPlaneRenderer::setSize(float size) { m_size = size; }

void DebugGroundPlaneRenderer::setQuadSize(float quadSize)
{
    m_quadSize = quadSize;
}

void DebugGroundPlaneRenderer::setRadius(float radius) { m_radius = radius; }

void DebugGroundPlaneRenderer::onRegisterRenderNodes()
{
    if (m_renderToGBuffer)
    {
        GetGlobal<RenderManager>()->registerRenderNode(
            std::make_shared<DebugGroundPlaneGBufferNode>(*this),
            RenderPhase::GBuffer);
    }
    else
    {
        GetGlobal<RenderManager>()->registerRenderNode(
            std::make_shared<DebugGroundPlaneForwardNode>(*this),
            RenderPhase::Forward);
    }
}
}
