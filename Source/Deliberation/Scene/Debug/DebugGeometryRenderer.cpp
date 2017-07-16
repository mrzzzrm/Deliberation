#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/Debug/DebugGeometryNode.h>

#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{
constexpr float DebugGeometryRenderer::ARROW_CONE_HEIGHT;

DebugGeometryRenderer::DebugGeometryRenderer(RenderManager & renderManager)
    : SingleNodeRenderer(renderManager, RenderPhase::PreOverlay, "DebugGeometryRenderer")
{
    auto & drawContext = renderManager.drawContext();
    
    /**
     * Load programs
     */
    m_buildIns.shadedProgram = drawContext.createProgram(
        {deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryShaded.vert"),
         deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryShaded.frag")});

    m_buildIns.unicolorProgram = drawContext.createProgram(
        {deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryUnicolor.vert"),
         deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryUnicolor.frag")});

    m_buildIns.vertexColorProgram = drawContext.createProgram(
        {deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryVertexColor.vert"),
         deliberation::DeliberationDataPath(
             "Data/Shaders/DebugGeometryVertexColor.frag")});

    /**
     * Create Box meshes
     */
    auto mesh = CuboidMesh({2.0f, 2.0f, 2.0f}).generate();
    {
        auto compilation =
            MeshCompiler().compile(mesh, MeshCompilerPrimitive::Triangles);

        m_buildIns.boxTrianglesVertexBuffer =
            drawContext.createBuffer(compilation.vertices);
        m_buildIns.boxTrianglesIndexBuffer =
            drawContext.createBuffer(compilation.indices);
    }

    {
        auto compilation =
            MeshCompiler().compile(mesh, MeshCompilerPrimitive::Lines);

        m_buildIns.boxLinesVertexBuffer =
            drawContext.createBuffer(compilation.vertices);
        m_buildIns.boxLinesIndexBuffer =
            drawContext.createBuffer(compilation.indices);
    }

    /**
     * Create Cone mesh
     */
    {
        auto mesh = ConeMesh(0.1f, ARROW_CONE_HEIGHT).generate();
        auto compilation =
            MeshCompiler().compile(mesh, MeshCompilerPrimitive::Triangles);

        m_buildIns.coneVertexBuffer =
            drawContext.createBuffer(compilation.vertices);
        m_buildIns.coneIndexBuffer =
            drawContext.createBuffer(compilation.indices);
    }

    /**
     * Create Sphere Mesh
     */
    {
        auto sphereMesh = UVSphere(16, 16).generateMesh2();
        m_buildIns.sphereVertexBuffer =
            drawContext.createBuffer(sphereMesh.takeVertices());
        m_buildIns.sphereIndexBuffer =
            drawContext.createBuffer(sphereMesh.takeIndices());
    }

    /**
     * Create Point Mesh
     */
    {
        LayoutedBlob vertex(DataLayout("Position", Type_Vec3), 1);
        vertex.field<glm::vec3>("Position")[0] = glm::vec3(0.0f, 0.0f, 0.0f);
        m_buildIns.pointVertexBuffer = drawContext.createBuffer(vertex);
    }

    /**
     * Create Data Layouts
     */
    m_buildIns.unicolorDataLayout = DataLayout("Position", Type_Vec3);
    m_buildIns.vertexColorDataLayout =
        DataLayout({{"Position", Type_Vec3}, {"Color", Type_Vec3}});
    m_buildIns.shadedDataLayout =
        DataLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});
}

std::shared_ptr<DebugGeometryNode> DebugGeometryRenderer::addNode()
{
    auto node = std::make_shared<DebugGeometryNode>(*this);
    m_nodes.emplace_back(node);
    return node;
}

void DebugGeometryRenderer::removeNode(const std::shared_ptr<DebugGeometryNode> & node)
{
    auto iter = std::find(m_nodes.begin(), m_nodes.end(), node);
    Assert(iter != m_nodes.end(), "");

    m_nodes.erase(iter);
}

void DebugGeometryRenderer::render()
{
    for (auto & node : m_nodes)
    {
        node->render(m_renderManager.mainCamera());
    }
}

}