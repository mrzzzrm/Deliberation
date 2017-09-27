#include <Deliberation/Scene/Debug/DebugCubemapRenderer.h>

#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{
DebugCubemapRenderer::DebugCubemapRenderer(
    const Camera3D & camera,
    const Texture &  cubemap,
    MeshType         meshType)
    : m_camera(camera)
{
    const auto program = GetGlobal<DrawContext>()->createProgram(
        {deliberation::DeliberationDataPath(
             "Data/Shaders/DebugCubemapRenderer.vert"),
         deliberation::DeliberationDataPath(
             "Data/Shaders/DebugCubemapRenderer.frag")});

    m_draw = GetGlobal<DrawContext>()->createDraw(program);

    if (meshType == MeshType::Cube)
    {
        LayoutedBlob vertices({"Position", Type_Vec3}, 8);
        vertices.field<glm::vec3>("Position")
            .assign(CuboidMesh::positions({2, 2, 2}));

        LayoutedBlob indices({"Index", Type_U32}, 36);
        indices.field<u32>("Index").assign(CuboidMesh::indices());

        m_draw.addVertices(vertices);
        m_draw.setIndices(indices);
    }
    else if (meshType == MeshType::Sphere)
    {
        auto mesh = UVSphere(12, 12).generateMesh2();
        m_draw.addVertices(mesh.vertices());
        m_draw.setIndices(mesh.indices());
    }

    m_draw.sampler("Texture").setTexture(cubemap);
    m_draw.state().setCullState(CullState::disabled());

    m_viewProjectionUniform = m_draw.uniform("ViewProjection");
    m_transformUniform = m_draw.uniform("Transform");
}

const Pose3D & DebugCubemapRenderer::pose() const { return m_pose; }

void DebugCubemapRenderer::setPose(const Pose3D & pose) { m_pose = pose; }

void DebugCubemapRenderer::render()
{
    m_viewProjectionUniform.set(m_camera.viewProjection());
    m_transformUniform.set(m_pose.matrix());
    m_draw.render();
}
}