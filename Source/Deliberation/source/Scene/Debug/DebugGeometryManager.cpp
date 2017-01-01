#include <Deliberation/Scene/Debug/DebugGeometryManager.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace
{

const float ARROW_CONE_HEIGHT = 0.2f;

}

namespace deliberation
{

DebugGeometryManager::DebugGeometryManager(Context & context):
    m_context(context)
{
    /**
     * Load programs
     */
    m_buildIns.shadedProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryShaded.vert"),
                                               deliberation::dataPath("Data/Shaders/DebugGeometryShaded.frag")});

    m_buildIns.unicolorProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.vert"),
                                                 deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.frag")});

    m_buildIns.vertexColorProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryVertexColor.vert"),
                                                    deliberation::dataPath("Data/Shaders/DebugGeometryVertexColor.frag")});


    /**
     * Create Box meshes
     */
    auto mesh = CuboidMesh({2.0f, 2.0f, 2.0f}).generate();
    {
        auto compilation = MeshCompiler().compile(mesh, Primitive_Triangles);

        m_buildIns.boxTrianglesVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_buildIns.boxTrianglesIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    {
        auto compilation = MeshCompiler().compile(mesh, Primitive_Lines);

        m_buildIns.boxLinesVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_buildIns.boxLinesIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    /**
     * Create Cone mesh
     */
    {
        auto mesh = ConeMesh(0.1f, ARROW_CONE_HEIGHT).generate();
        auto compilation = MeshCompiler().compile(mesh, Primitive_Triangles);

        m_buildIns.coneVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_buildIns.coneIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    /**
     * Create Sphere Mesh
     */
    {
        auto sphereMesh = UVSphere(6, 6).generateMesh2();
        m_buildIns.sphereVertexBuffer = m_context.createBuffer(sphereMesh.takeVertices());
        m_buildIns.sphereIndexBuffer = m_context.createBuffer(sphereMesh.takeIndices());
    }

    /**
     * Create Data Layouts
     */
    m_buildIns.unicolorDataLayout = DataLayout("Position", Type_Vec3);
    m_buildIns.vertexColorDataLayout = DataLayout({{"Position", Type_Vec3}, {"Color", Type_Vec3}});
    m_buildIns.shadedDataLayout = DataLayout({{"Position", Type_Vec3}, {"Normal", Type_Vec3}});
}

Context & DebugGeometryManager::context() const
{
    return m_context;
}

const DebugGeometryManager::BuildIns & DebugGeometryManager::buildIns() const
{
    return m_buildIns;
}

}