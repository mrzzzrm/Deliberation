#include <Deliberation/Scene/Debug/DebugGroundPlaneRenderer.h>

#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

DebugGroundPlaneRenderer::DebugGroundPlaneRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{
    m_program = m_context.createProgram({deliberation::DeliberationDataPath("Data/Shaders/DebugGroundPlaneRenderer.vert"),
                                         deliberation::DeliberationDataPath("Data/Shaders/DebugGroundPlaneRenderer.frag")});
    m_draw = m_context.createDraw(m_program, gl::GL_TRIANGLE_STRIP);

    m_view = m_draw.uniform("View");
    m_projection = m_draw.uniform("Projection");

    m_size = m_draw.uniform("Size");
    m_size.set(3.0f);

    m_quadSize = m_draw.uniform("QuadSize");
    m_quadSize.set(0.5f);

    m_radius = m_draw.uniform("Radius");
    m_radius.set(3.0f);

    LayoutedBlob vertices({"Position", Type_Vec3}, 4);

    auto positions = vertices.field<glm::vec3>("Position");

    positions[0] = {-1.0f, 0.0f, 1.0f};
    positions[1] = {-1.0f, 0.0f,-1.0f};
    positions[2] = { 1.0f, 0.0f, 1.0f};
    positions[3] = { 1.0f, 0.0f,-1.0f};

    m_draw.state().setCullState(CullState::disabled());
    m_draw.addVertices(vertices);
}

void DebugGroundPlaneRenderer::setFramebuffer(Framebuffer & framebuffer)
{
    m_draw.setFramebuffer(framebuffer);
}

void DebugGroundPlaneRenderer::setSize(float size)
{
    m_size.set(size);
}

void DebugGroundPlaneRenderer::setQuadSize(float quadSize)
{
    m_quadSize.set(quadSize);
}

void DebugGroundPlaneRenderer::setRadius(float radius)
{
    m_radius.set(radius);
}

void DebugGroundPlaneRenderer::render()
{
    m_view.set(m_camera.view());
    m_projection.set(m_camera.projection());

    m_draw.schedule();
}


}

