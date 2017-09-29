#include <Deliberation/Scene/SkyboxRenderer.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CuboidMesh.h>

namespace deliberation
{
SkyboxRenderer::SkyboxRenderer(const Texture & cubemap)
    : SingleNodeRenderer(RenderPhase::Forward, "Skybox")
    , m_cubemap(cubemap)
{
    m_program = GetGlobal<DrawContext>()->createProgram(
        {deliberation::DeliberationDataPath("Data/Shaders/SkyboxRenderer.vert"),
         deliberation::DeliberationDataPath(
             "Data/Shaders/SkyboxRenderer.frag")});

    //    auto vertices = LayoutedBlob(vertexLayout, 4);
    //    vertices.field<glm::vec3>("Position").assign({
    //        glm::vec3(-1.0f, -1.0f, 0.0f),
    //        glm::vec3( 1.0f, -1.0f, 0.0f),
    //        glm::vec3( 1.0f,  1.0f, 0.0f),
    //        glm::vec3(-1.0f,  1.0f, 0.0f)});
    //
    //    auto indices = LayoutedBlob(DataLayout("Index", Type_U32), 6);
    //    indices.field<u32>("Index").assign({
    //        0, 1, 3, 1, 2, 3});
}

void SkyboxRenderer::render()
{
    if (m_drawDirty)
    {
        m_draw = GetGlobal<DrawContext>()->createDraw(m_program);

        const auto vertexLayout = DataLayout{{{"Position", Type_Vec3}}};

        LayoutedBlob vertices({"Position", Type_Vec3}, 8);
        vertices.field<glm::vec3>("Position")
            .assign(CuboidMesh::positions({2, 2, 2}));

        LayoutedBlob indices({"Index", Type_U32}, 36);
        indices.field<u32>("Index").assign(CuboidMesh::indices());

        m_draw.addVertices(vertices);
        m_draw.setIndices(indices);
        m_draw.sampler("Texture").setTexture(m_cubemap);
        m_draw.state().setDepthState(
            {DepthTest::LessOrEqual, DepthWrite::Disabled});
        m_draw.state().setCullState(CullState::disabled());
        m_draw.setFramebuffer(GetGlobal<RenderManager>()->hdrBuffer());

        m_drawDirty = false;
    }

    auto view = glm::mat4(glm::mat3(
        GetGlobal<RenderManager>()->mainCamera()
            .view())); // Remove any translation component of the view matrix

    m_draw.uniform("View").set(view);
    m_draw.uniform("Projection").set(GetGlobal<RenderManager>()->mainCamera().projection());
    m_draw.render();
}
}