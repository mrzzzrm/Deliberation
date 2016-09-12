#include <Deliberation/Scene/DebugGeometryRenderer.h>

#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>

namespace deliberation
{

DebugBoxInstance::DebugBoxInstance(const Draw & draw,
                                   const glm::vec3 & halfExtent,
                                   const glm::vec3 & color):
    m_draw(draw),
    m_halfExtent(halfExtent),
    m_color(color)
{
    m_draw.uniform("Color").set(color);
    m_draw.uniform("Scale").set(halfExtent);
}

Transform3D & DebugBoxInstance::transform()
{
    return m_transform;
}

Box DebugBoxInstance::toBox() const
{
    return Box(m_halfExtent, m_transform);
}

DebugPointInstance::DebugPointInstance(Program & program,
                                       const DataLayout & vertexLayout,
                                       const glm::vec3 & position,
                                       const glm::vec3 & color):
    m_position(position),
    m_color(color)
{
    auto & context = program.context();
    m_draw = context.createDraw(program, gl::GL_POINTS);

    m_draw.uniform("Transform").set(glm::mat4(1.0f));
    m_draw.uniform("Color").set(color);
    m_draw.uniform("Scale").set(glm::vec3(1.0f));
    m_draw.state().rasterizerState().setPointSize(3.0f);

    m_vertexBuffer = context.createBuffer(vertexLayout);
    m_vertexBuffer.scheduleRawUpload(Blob::fromValue(position), 1);

    m_draw.addVertexBuffer(m_vertexBuffer);
}

void DebugPointInstance::setPosition(const glm::vec3 & position)
{
    if (m_position == position)
    {
        return;
    }

    m_vertexBuffer.scheduleRawUpload(Blob::fromValue(position), 1);
}


DebugArrowInstance::DebugArrowInstance(const Program & program,
                                       const DataLayout & vertexLayout,
                                       const glm::vec3 & origin,
                                       const glm::vec3 & delta,
                                       const glm::vec3 & color):
    m_origin(origin),
    m_delta(delta),
    m_color(color)
{
    auto & context = program.context();
    m_draw = context.createDraw(program, gl::GL_LINES);

    m_draw.uniform("Transform").set(glm::mat4(1.0f));
    m_draw.uniform("Color").set(color);
    m_draw.uniform("Scale").set(glm::vec3(1.0f));

    m_vertexBuffer = context.createBuffer(vertexLayout);

    buildVertices();

    m_draw.addVertexBuffer(m_vertexBuffer);
}

void DebugArrowInstance::reset(const glm::vec3 & origin, const glm::vec3 & delta)
{
    if (m_origin == origin && m_delta == delta)
    {
        return;
    }

    m_origin = origin;
    m_delta = delta;

    buildVertices();
}

void DebugArrowInstance::buildVertices()
{
    LayoutedBlob vertices(m_vertexBuffer.layout(), 2);
    auto positions = vertices.field<glm::vec3>("Position");

    positions[0] = m_origin;
    positions[1] = m_origin + m_delta;

    m_vertexBuffer.scheduleUpload(vertices);
}

DebugGeometryRenderer::DebugGeometryRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{


    m_shadedProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryShaded.vert"),
                                               deliberation::dataPath("Data/Shaders/DebugGeometryShaded.frag")});

    m_unicolorProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.vert"),
                                                 deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.frag")});

    auto mesh = CuboidMesh({2.0f, 2.0f, 2.0f}).generate();
    {
        auto compilation = MeshCompiler().compile(mesh, Primitive_Triangles);

        m_boxTrianglesVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_boxTrianglesIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    {
        auto compilation = MeshCompiler().compile(mesh, Primitive_Lines);

        m_boxLinesVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_boxLinesIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    m_unicolorDataLayout = DataLayout("Position", Type_Vec3);
}

DebugBoxInstance & DebugGeometryRenderer::box(size_t index)
{
    Assert(index < m_boxes.size(), " ");
    return m_boxes[index];
}

DebugArrowInstance & DebugGeometryRenderer::arrow(size_t index)
{
    Assert(index < m_arrows.size(), " ");
    return m_arrows[index];
}

DebugPointInstance & DebugGeometryRenderer::point(size_t index)
{
    Assert(index < m_points.size(), " ");
    return m_points[index];
}

size_t DebugGeometryRenderer::addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe)
{
    Draw draw;

    if (wireframe)
    {
        draw = m_context.createDraw(m_unicolorProgram, gl::GL_LINES);
        draw.addVertexBuffer(m_boxLinesVertexBuffer);
        draw.setIndexBuffer(m_boxLinesIndexBuffer);
    }
    else
    {
        draw = m_context.createDraw(m_shadedProgram, gl::GL_LINES);
        draw.addVertexBuffer(m_boxTrianglesVertexBuffer);
        draw.setIndexBuffer(m_boxTrianglesIndexBuffer);
    }

    m_boxes.push_back({draw, halfExtent, color});

    return m_boxes.size() - 1;
}

size_t DebugGeometryRenderer::addPoint(const glm::vec3 & position, const glm::vec3 & color)
{
    m_points.push_back({m_unicolorProgram, m_unicolorDataLayout, position, color});
    return m_points.size() - 1;
}

size_t DebugGeometryRenderer::addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color)
{
    m_arrows.push_back({m_unicolorProgram, m_unicolorDataLayout, origin, delta, color});
    return m_arrows.size() - 1;
}

void DebugGeometryRenderer::removeBox(size_t index)
{
    Assert(index < m_boxes.size(), " ");
    m_boxes.erase(m_boxes.begin() + index);
}

void DebugGeometryRenderer::removePoint(size_t index)
{
    Assert(index < m_points.size(), " ");
    m_points.erase(m_points.begin() + index);
}

void DebugGeometryRenderer::removeArrow(size_t index)
{
    Assert(index < m_arrows.size(), " ");
    m_arrows.erase(m_arrows.begin() + index);
}

void DebugGeometryRenderer::schedule()
{
    for (auto & box : m_boxes)
    {
        auto & draw = box.m_draw;

        draw.uniform("Transform").set(box.m_transform.matrix());
        draw.uniform("ViewProjection").set(m_camera.viewProjection());

        draw.schedule();
    }
    for (auto & point : m_points)
    {
        auto & draw = point.m_draw;
        draw.uniform("ViewProjection").set(m_camera.viewProjection());
        draw.schedule();
    }
    for (auto & arrow : m_arrows)
    {
        auto & draw = arrow.m_draw;

        draw.uniform("ViewProjection").set(m_camera.viewProjection());

        draw.schedule();
    }
}

}
