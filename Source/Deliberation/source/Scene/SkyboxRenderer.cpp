#include <Deliberation/Scene/SkyboxRenderer.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

SkyboxRenderer::SkyboxRenderer(Context & context, const Camera3D & camera, const Texture & cubemap):
    m_camera(camera)
{
    const auto program = context.createProgram({
        deliberation::dataPath("Data/Shaders/SkyboxRenderer.vert"),
        deliberation::dataPath("Data/Shaders/SkyboxRenderer.frag")});

    m_draw = context.createDraw(program, gl::GL_TRIANGLES);

    const auto vertexLayout = DataLayout{{{"Position", Type_Vec3}}};

    auto vertices = LayoutedBlob(vertexLayout, 4);

    vertices.field<glm::vec3>("Position").assign({
        glm::vec3(-1.0f, -1.0f, 0.0f),
        glm::vec3( 1.0f, -1.0f, 0.0f),
        glm::vec3( 1.0f,  1.0f, 0.0f),
        glm::vec3(-1.0f,  1.0f, 0.0f)});

    auto indices = LayoutedBlob(DataLayout("Index", Type_U32), 6);
    indices.field<u32>("Index").assign({
        0, 1, 3, 1, 2, 3});

    m_draw.addVertices(vertices);
    m_draw.setIndices(indices);
    m_draw.sampler("Texture").setTexture(cubemap);
    m_draw.state().setDepthState({true, false});
}

void SkyboxRenderer::render()
{
    m_draw.uniform("ViewProjectionInverted").set(glm::mat4_cast(m_camera.orientation()) * glm::inverse(m_camera.projection()));
    m_draw.schedule();
}

}