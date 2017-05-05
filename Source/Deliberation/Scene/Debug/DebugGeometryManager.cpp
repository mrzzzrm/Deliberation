#include <Deliberation/Scene/Debug/DebugGeometryManager.h>

#include <Deliberation/Draw/DrawContext.h>

#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{

constexpr float DebugGeometryManager::ARROW_CONE_HEIGHT;

DebugGeometryManager::DebugGeometryManager(DrawContext & drawContext):
    m_drawContext(drawContext)
{
    /**
     * Load programs
     */
    m_buildIns.shadedProgram = m_drawContext.createProgram({deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryShaded.vert"),
                                               deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryShaded.frag")});

    m_buildIns.unicolorProgram = m_drawContext.createProgram({deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryUnicolor.vert"),
                                                 deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryUnicolor.frag")});

    m_buildIns.vertexColorProgram = m_drawContext.createProgram({deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryVertexColor.vert"),
                                                    deliberation::DeliberationDataPath("Data/Shaders/DebugGeometryVertexColor.frag")});


    /**
     * Create Box meshes
     */
    auto mesh = CuboidMesh({2.0f, 2.0f, 2.0f}).generate();
    {
        auto compilation = MeshCompiler().compile(mesh, MeshCompilerPrimitive::Triangles);

        m_buildIns.boxTrianglesVertexBuffer = m_drawContext.createBuffer(compilation.vertices);
        m_buildIns.boxTrianglesIndexBuffer = m_drawContext.createBuffer(compilation.indices);
    }

    {
        auto compilation = MeshCompiler().compile(mesh, MeshCompilerPrimitive::Lines);

        m_buildIns.boxLinesVertexBuffer = m_drawContext.createBuffer(compilation.vertices);
        m_buildIns.boxLinesIndexBuffer = m_drawContext.createBuffer(compilation.indices);
    }

    /**
     * Create Cone mesh
     */
    {
        auto mesh = ConeMesh(0.1f, ARROW_CONE_HEIGHT).generate();
        auto compilation = MeshCompiler().compile(mesh, MeshCompilerPrimitive::Triangles);

        m_buildIns.coneVertexBuffer = m_drawContext.createBuffer(compilation.vertices);
        m_buildIns.coneIndexBuffer = m_drawContext.createBuffer(compilation.indices);
    }

    /**
     * Create Sphere Mesh
     */
    {
        auto sphereMesh = UVSphere(16, 16).generateMesh2();
        m_buildIns.sphereVertexBuffer = m_drawContext.createBuffer(sphereMesh.takeVertices());
        m_buildIns.sphereIndexBuffer = m_drawContext.createBuffer(sphereMesh.takeIndices());
    }

    /**
     * Create Point Mesh
     */
    {
        LayoutedBlob vertex(DataLayout("Position", Type_Vec3), 1);
        vertex.field<glm::vec3>("Position")[0] = glm::vec3(0.0f, 0.0f, 0.0f);
        m_buildIns.pointVertexBuffer = m_drawContext.createBuffer(vertex);
    }

    /**
     * Create Data Layouts
     */
    m_buildIns.unicolorDataLayout = DataLayout("Position", Type_Vec3);
    m_buildIns.vertexColorDataLayout = DataLayout({{"Position", Type_Vec3}, {"Color", Type_Vec3}});
    m_buildIns.shadedDataLayout = DataLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});
}

DrawContext & DebugGeometryManager::drawContext() const
{
    return m_drawContext;
}

const DebugGeometryManager::BuildIns & DebugGeometryManager::buildIns() const
{
    return m_buildIns;
}

}