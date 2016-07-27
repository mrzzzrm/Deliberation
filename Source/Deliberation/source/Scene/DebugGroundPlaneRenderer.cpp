#include <Deliberation/Scene/DebugGroundPlaneRenderer.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

DebugGroundPlaneRenderer::DebugGroundPlaneRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{
    m_program = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGroundPlaneRenderer.vert"),
                                         deliberation::dataPath("Data/Shaders/DebugGroundPlaneRenderer.frag")});
    m_draw = m_context.createDraw(m_program, gl::GL_TRIANGLE_STRIP);

    m_view = m_draw.uniform("View");
    m_projection = m_draw.uniform("Projection");
    m_farPlaneZ = m_draw.uniform("FarPlaneZ");

    m_size = m_draw.uniform("Size");
    m_size.set(3.0f);

    m_quadSize = m_draw.uniform("QuadSize");
    m_quadSize.set(0.5f);

    std::vector<glm::vec3> vertices({
        {-1.0f, 0.0f, 1.0f},
        {-1.0f, 0.0f,-1.0f},
        { 1.0f, 0.0f, 1.0f},
        { 1.0f, 0.0f,-1.0f}
    });

    m_draw.state().setCullState(CullState::disabled());
    m_draw.addVertices(DataLayout("Position", Type_Vec3), vertices);
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

void DebugGroundPlaneRenderer::schedule()
{
    m_view.set(m_camera.view());
    m_projection.set(m_camera.projection());
    m_farPlaneZ.set(m_camera.zFar());

    m_draw.schedule();
}


}

