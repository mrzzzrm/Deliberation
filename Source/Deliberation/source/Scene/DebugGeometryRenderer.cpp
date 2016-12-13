#include <Deliberation/Scene/DebugGeometryRenderer.h>

#include <iostream>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Draw/Context.h>

#include <Deliberation/Scene/Camera3D.h>
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

bool DebugGeometryInstance::visible() const
{
    return m_visible;
}

void DebugGeometryInstance::setVisible(bool visible)
{
    m_visible = visible;
}

DebugGeometryInstance::DebugGeometryInstance() = default;

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

void DebugBoxInstance::setTransform(const Transform3D & transform)
{
    m_transform = transform;
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


DebugArrowInstance::DebugArrowInstance(const Program & lineProgram,
                                       const DataLayout & lineVertexLayout,
                                       const Draw & coneDraw,
                                       const glm::vec3 & origin,
                                       const glm::vec3 & delta,
                                       const glm::vec3 & color):
    m_origin(origin),
    m_delta(delta),
    m_color(color)
{
    auto & context = lineProgram.context();

    /**
     * Setup line draw
     */
    m_lineDraw = context.createDraw(lineProgram, gl::GL_LINES);

    m_lineDraw.uniform("Transform").set(glm::mat4(1.0f));
    m_lineDraw.uniform("Color").set(color);
    m_lineDraw.uniform("Scale").set(glm::vec3(1.0f));

    m_lineVertexBuffer = context.createBuffer(lineVertexLayout);

    buildVertices();

    m_lineDraw.addVertexBuffer(m_lineVertexBuffer);

    /**
     * Setup cone Draw
     */
    m_coneDraw = coneDraw;
    setupConeTransform();
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
    setupConeTransform();
}

void DebugArrowInstance::setColor(const glm::vec3 & color)
{
    m_lineDraw.uniform("Color").set(color);
    m_coneDraw.uniform("Color").set(color);
}

void DebugArrowInstance::buildVertices()
{
    LayoutedBlob vertices(m_lineVertexBuffer.layout(), 2);
    auto positions = vertices.field<glm::vec3>("Position");

    positions[0] = m_origin;
    positions[1] = m_origin + m_delta;

    m_lineVertexBuffer.scheduleUpload(vertices);
}

void DebugArrowInstance::setupConeTransform()
{
    if (m_delta == glm::vec3(0.0f))
    {
        m_coneDraw.uniform("Transform").set(Transform3D::identity().matrix());
        return;
    }

    Transform3D transform;

    auto rotation = RotationMatrixFromDirectionY(m_delta);

    transform.setPosition(m_origin + m_delta - glm::normalize(m_delta) * ARROW_CONE_HEIGHT);
    transform.setOrientation(glm::quat_cast(rotation));

    m_coneDraw.uniform("Transform").set(transform.matrix());
}

DebugWireframeInstance::DebugWireframeInstance(const Program & program,
                                               const DataLayout & vertexLayout):
    m_program(program),
    m_vertexLayout(vertexLayout)
{

}

void DebugWireframeInstance::setTransform(const Transform3D & transform)
{
    m_transform = transform;
    if (m_draw.engaged())
    {
        m_draw->uniform("Transform").set(m_transform.matrix());
    }
}

void DebugWireframeInstance::addLineStrip(const std::vector<ColoredVertex> & vertices)
{
    Assert(vertices.size() >= 2, "Not a line strip");

    m_lineStrips.push_back(vertices);
    m_lineCount += vertices.size() - 1;
    m_dirty = true;
}

void DebugWireframeInstance::schedule(const Camera3D & camera)
{
    if (m_lineStrips.empty())
    {
        return;
    }

    if (m_dirty)
    {
        m_draw.reset(m_program.context().createDraw(m_program, gl::GL_LINES));

        auto vertices = LayoutedBlob(m_vertexLayout, m_lineCount * 2);
        auto positions = vertices.field<glm::vec3>("Position");
        auto colors = vertices.field<glm::vec3>("Color");

        u32 i = 0;

        for (auto & strip : m_lineStrips)
        {
            for (u32 v = 0; v < strip.size() - 1; v++)
            {
                positions[i + 0] = strip[v].position;
                positions[i + 1] = strip[v + 1].position;
                colors[i + 0] = strip[v].color;
                colors[i + 1] = strip[v + 1].color;

                i += 2;
            }
        }

        m_draw->addVertices(vertices);
        m_draw->uniform("Transform").set(m_transform.matrix());

        m_dirty = false;
    }

    m_draw->uniform("ViewProjection").set(camera.viewProjection());
    m_draw->schedule();
}

DebugSphereInstance::DebugSphereInstance(const Program & program,
                                         const Buffer & vertexBuffer,
                                         const Buffer & indexBuffer,
                                         const glm::vec3 & color,
                                         const float scale):
    m_program(program),
    m_vertexBuffer(vertexBuffer),
    m_indexBuffer(indexBuffer),
    m_color(color),
    m_scale(scale)
{

}

void DebugSphereInstance::setTransform(const Transform3D & transform)
{
    m_transform = transform;
    if (m_draw.engaged())
    {
        m_draw->uniform("Transform").set(m_transform.scaled(m_scale).matrix());
    }
}

void DebugSphereInstance::schedule(const Camera3D & camera)
{
    if (m_dirty)
    {
        m_draw.reset(m_program.context().createDraw(m_program, gl::GL_TRIANGLES));

        m_draw->addVertexBuffer(m_vertexBuffer);
        m_draw->setIndexBuffer(m_indexBuffer);
        m_draw->uniform("Color").set(m_color);
        m_draw->uniform("Transform").set(m_transform.scaled(m_scale).matrix());

        m_dirty = false;
    }

    m_draw->uniform("ViewProjection").set(camera.viewProjection());
    m_draw->schedule();
}

DebugGeometryRenderer::DebugGeometryRenderer(Context & context, const Camera3D & camera):
    m_context(context),
    m_camera(camera)
{


    m_shadedProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryShaded.vert"),
                                               deliberation::dataPath("Data/Shaders/DebugGeometryShaded.frag")});

    m_unicolorProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.vert"),
                                                 deliberation::dataPath("Data/Shaders/DebugGeometryUnicolor.frag")});

    m_vertexColorProgram = m_context.createProgram({deliberation::dataPath("Data/Shaders/DebugGeometryVertexColor.vert"),
                                                    deliberation::dataPath("Data/Shaders/DebugGeometryVertexColor.frag")});

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

    /**
     * Create cone mesh
     */
    {
        auto mesh = ConeMesh(0.1f, ARROW_CONE_HEIGHT).generate();
        auto compilation = MeshCompiler().compile(mesh, Primitive_Triangles);

        m_coneVertexBuffer = m_context.createBuffer(compilation.vertices);
        m_coneIndexBuffer = m_context.createBuffer(compilation.indices);
    }

    /**
     * Create Sphere Mesh
     */
    {
        auto sphereMesh = UVSphere(6, 6).generateMesh2();
        m_sphereVertexBuffer = m_context.createBuffer(sphereMesh.takeVertices());
        m_sphereIndexBuffer = m_context.createBuffer(sphereMesh.takeIndices());
    }

    /**
     *
     */
    m_unicolorDataLayout = DataLayout("Position", Type_Vec3);
    m_vertexColorDataLayout = DataLayout({{"Position", Type_Vec3}, {"Color", Type_Vec3}});
}


bool DebugGeometryRenderer::visible() const
{
    return m_visible;
}

void DebugGeometryRenderer::setVisible(bool visible)
{
    m_visible = visible;
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

DebugWireframeInstance & DebugGeometryRenderer::wireframe(size_t index)
{
    Assert(index < m_wireframes.size(), " ");
    return m_wireframes[index];
}

DebugSphereInstance & DebugGeometryRenderer::sphere(size_t index)
{
    Assert(index < m_spheres.size(), " ");
    return m_spheres[index];
}

void DebugGeometryRenderer::resizeArrows(uint count, const glm::vec3 & color, bool visible)
{
    if (count > m_arrows.size())
    {
        allocateArrows(count - m_arrows.size(), color, visible);
    }
    else
    {
        m_arrows.erase(m_arrows.begin() + count, m_arrows.end());
        for (auto & arrow : m_arrows)
        {
            // TODO: Set color
            arrow.setVisible(visible);
        }
    }
}

//void DebugGeometryRenderer::allocateBoxes(uint count, bool wireframe)
//{
//
//}

void DebugGeometryRenderer::allocatePoints(uint count, const glm::vec3 & color, bool visible)
{
    for (uint p = 0; p < count; p++)
    {
        auto index = addPoint({}, color);
        point(index).setVisible(visible);
    }
}

void DebugGeometryRenderer::allocateArrows(uint count, const glm::vec3 & color, bool visible)
{
    for (uint a = 0; a < count; a++)
    {
        auto index = addArrow({}, {}, color);
        arrow(index).setVisible(visible);
    }
}
//void DebugGeometryRenderer::allocateArrows(uint count)
//{

//}


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
        draw = m_context.createDraw(m_shadedProgram, gl::GL_TRIANGLES);
        draw.addVertexBuffer(m_boxTrianglesVertexBuffer);
        draw.setIndexBuffer(m_boxTrianglesIndexBuffer);
    }

    draw.state().rasterizerState().setLineWidth(2.0f);

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
    auto draw = m_context.createDraw(m_shadedProgram, gl::GL_TRIANGLES);
    draw.addVertexBuffer(m_coneVertexBuffer);
    draw.setIndexBuffer(m_coneIndexBuffer);
    draw.uniform("Color").set(color);
    draw.uniform("Scale").set(glm::vec3(1.0f, 1.0f, 1.0f));

    m_arrows.push_back({m_unicolorProgram, m_unicolorDataLayout, draw, origin, delta, color});
    return m_arrows.size() - 1;
}

size_t DebugGeometryRenderer::addWireframe()
{
    m_wireframes.emplace_back(m_vertexColorProgram, m_vertexColorDataLayout);
    return m_wireframes.size() - 1;
}

size_t DebugGeometryRenderer::addSphere(const glm::vec3 & color, float radius)
{
    m_spheres.emplace_back(m_shadedProgram, m_sphereVertexBuffer, m_sphereIndexBuffer, color, radius);
    return m_spheres.size() - 1;

}

DebugBoxInstance & DebugGeometryRenderer::addAndGetBox(const glm::vec3 & halfExtent,
                                                       const glm::vec3 & color,
                                                       bool wireframe)
{
    return box(addBox(halfExtent, color, wireframe));
}

DebugWireframeInstance & DebugGeometryRenderer::addAndGetWireframe()
{
    return wireframe(addWireframe());
}

DebugSphereInstance & DebugGeometryRenderer::addAndGetSphere(const glm::vec3 & color, float radius)
{
    return sphere(addSphere(color, radius));
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
    if (!m_visible)
    {
        return;
    }

    for (auto & box : m_boxes)
    {
        if (!box.visible())
        {
            continue;
        }

        auto & draw = box.m_draw;

        draw.uniform("Transform").set(box.m_transform.matrix());
        draw.uniform("ViewProjection").set(m_camera.viewProjection());

        draw.schedule();
    }

    for (auto & point : m_points)
    {
        if (!point.visible())
        {
            continue;
        }
        auto & draw = point.m_draw;
        draw.uniform("ViewProjection").set(m_camera.viewProjection());
        draw.schedule();
    }

    for (auto & arrow : m_arrows)
    {
        if (!arrow.visible())
        {
            continue;
        }

        auto & lineDraw = arrow.m_lineDraw;
        lineDraw.uniform("ViewProjection").set(m_camera.viewProjection());
        lineDraw.schedule();

        auto & coneDraw = arrow.m_coneDraw;
        coneDraw.uniform("ViewProjection").set(m_camera.viewProjection());
        coneDraw.schedule();
    }

    for (auto & wireframe : m_wireframes)
    {
        wireframe.schedule(m_camera);
    }

    for (auto & sphere : m_spheres)
    {
        sphere.schedule(m_camera);
    }
}

}
