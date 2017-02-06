#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include <iostream>
#include <memory>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Draw/Context.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{

DebugGeometryInstance::DebugGeometryInstance(DebugGeometryRenderer & renderer,
                                             size_t index):
    m_renderer(renderer),
    m_index(index)
{}

size_t DebugGeometryInstance::index() const
{
    return m_index;
}

DebugGeometryRenderer & DebugGeometryInstance::renderer() const
{
    return m_renderer;
}

DebugGeometryManager & DebugGeometryInstance::manager() const
{
    return m_renderer.manager();
}

const DebugGeometryManager::BuildIns & DebugGeometryInstance::buildIns() const
{
    return manager().buildIns();
}

bool DebugGeometryInstance::visible() const
{
    return m_visible;
}

const Transform3D & DebugGeometryInstance::transform() const
{
    return m_transform;
}

void DebugGeometryInstance::setVisible(bool visible)
{
    m_visible = visible;
}

void DebugGeometryInstance::setTransform(const Transform3D & transform)
{
    m_transform = transform;
    m_transformDirty = true;
}

DebugBoxInstance::DebugBoxInstance(DebugGeometryRenderer & renderer,
                                   size_t index):
    DebugGeometryInstance(renderer, index)
{}

bool DebugBoxInstance::wireframe() const
{
    return m_wireframe;
}

const glm::vec3 & DebugBoxInstance::halfExtent() const
{
    return m_halfExtent;
}

const glm::vec3 & DebugBoxInstance::color() const
{
    return m_color;
}

void DebugBoxInstance::setHalfExtent(const glm::vec3 & halfExtent)
{
    m_halfExtent = halfExtent;
    m_meshDirty = true;
}

void DebugBoxInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorDirty = true;
}

void DebugBoxInstance::setWireframe(bool wireframe)
{
    m_wireframe = wireframe;
    m_meshDirty = true; 
}

void DebugBoxInstance::schedule() const
{
    if (m_meshDirty)
    {
        if (m_wireframe) {
            m_draw = manager().context().createDraw(
                buildIns().unicolorProgram,
                gl::GL_LINES
            );
            m_draw.addVertexBuffer(buildIns().boxLinesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxLinesIndexBuffer);
        } else {
            m_draw = manager().context().createDraw(
                buildIns().shadedProgram,
                gl::GL_TRIANGLES
            );
            m_draw.addVertexBuffer(buildIns().boxTrianglesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxTrianglesIndexBuffer);
        }

        m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

        m_transformUniform = m_draw.uniform("Transform");

        m_meshDirty = false;
        m_colorDirty = true;
    }

    if (m_colorDirty)
    {
        m_draw.uniform("Color").set(m_color);
        m_colorDirty = false;
    }

    if (m_transformDirty)
    {
        m_transformUniform.set(m_transform.matrix() * glm::scale(m_halfExtent));
        m_transformDirty = false;
    }

    m_draw.schedule();
}

Box DebugBoxInstance::toBox() const
{
    return Box(m_halfExtent, m_transform);
}

DebugPointInstance::DebugPointInstance(DebugGeometryRenderer & renderer,
                                       size_t index):
    DebugGeometryInstance(renderer, index)
{
    m_draw = manager().context().createDraw(manager().buildIns().unicolorProgram, gl::GL_POINTS);
    m_draw.addVertexBuffer(manager().buildIns().pointVertexBuffer);;
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_draw.state().rasterizerState().setPointSize(3.0f);
    m_transformUniform = m_draw.uniform("Transform");

    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 & DebugPointInstance::color() const
{
    return m_color;
}

void DebugPointInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugPointInstance::schedule() const
{
    if (m_transformDirty)
    {
        m_transformUniform.set(m_transform.matrix());
        m_transformDirty = false;
    }

    m_draw.schedule();
}

DebugArrowInstance::DebugArrowInstance(DebugGeometryRenderer & renderer,
                                       size_t index):
    DebugGeometryInstance(renderer, index)
{
    m_lineVertices = LayoutedBlob(DataLayout("Position", Type_Vec3), 2);
    m_lineVertexBuffer = manager().context().createBuffer(m_lineVertices.layout());

    m_lineDraw = manager().context().createDraw(manager().buildIns().unicolorProgram,
                                                gl::GL_LINES);
    m_lineDraw.addVertexBuffer(m_lineVertexBuffer);
    m_lineDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_lineDraw.uniform("Transform").set(glm::mat4(1.0f));
    m_lineColorUniform = m_lineDraw.uniform("Color");

    m_coneDraw = manager().context().createDraw(manager().buildIns().shadedProgram,
                                                gl::GL_TRIANGLES);
    m_coneDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_coneDraw.addVertexBuffer(manager().buildIns().coneVertexBuffer);
    m_coneDraw.setIndexBuffer(manager().buildIns().coneIndexBuffer);
    m_coneColorUniform = m_coneDraw.uniform("Color");
    m_coneTransformUniform = m_coneDraw.uniform("Transform");
}

const glm::vec3 & DebugArrowInstance::origin() const
{
    return m_origin;
}

const glm::vec3 & DebugArrowInstance::delta() const
{
    return m_delta;
}

const glm::vec3 & DebugArrowInstance::color() const
{
    return m_color;
}

void DebugArrowInstance::setOrigin(const glm::vec3 & origin)
{
    reset(origin, m_delta);
}

void DebugArrowInstance::setDelta(const glm::vec3 & delta)
{
    reset(m_origin, delta);
}

void DebugArrowInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_lineColorUniform.set(m_color);
    m_coneColorUniform.set(m_color);
}

void DebugArrowInstance::reset(const glm::vec3 & origin, const glm::vec3 & delta)
{
    m_origin = origin;
    m_delta = delta;

    auto positions = m_lineVertices.field<glm::vec3>("Position");
    positions[0] = m_origin;
    positions[1] = m_origin + m_delta;

    m_lineVertexBuffer.scheduleUpload(m_lineVertices);

    if (glm::length2(m_delta) < 0.001f)
    {
        m_coneTransformUniform.set(Transform3D::identity().matrix());
        return;
    }

    Transform3D transform;

    auto rotation = RotationMatrixFromDirectionY(m_delta);

    transform.setPosition(m_origin + m_delta - glm::normalize(m_delta) * DebugGeometryManager::ARROW_CONE_HEIGHT);
    transform.setOrientation(glm::quat_cast(rotation));

    m_coneTransformUniform.set(transform.matrix());
}

void DebugArrowInstance::schedule() const
{
    m_lineDraw.schedule();
    m_coneDraw.schedule();
}

DebugWireframeInstance::DebugWireframeInstance(DebugGeometryRenderer & renderer,
                                               size_t index):
    DebugGeometryInstance(renderer, index)
{
    m_vertices = LayoutedBlob(DataLayout("Position", Type_Vec3));

    m_vertexBuffer = manager().context().createBuffer(m_vertices.layout());

    m_draw = manager().context().createDraw(manager().buildIns().unicolorProgram, gl::GL_LINES);
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_transformUniform = m_draw.uniform("Transform");
    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 & DebugWireframeInstance::color() const
{
    return m_color;
}

void DebugWireframeInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugWireframeInstance::addLineStrip(const std::vector<BasicVertex> & vertices)
{
    Assert(vertices.size() >= 2, "Not a line strip");

    auto oldNumVertices = m_vertices.count();

    m_vertices.resize(oldNumVertices + (vertices.size() - 1) * 2);

    auto positions = m_vertices.field<glm::vec3>("Position");

    for (uint l = 0; l < vertices.size() - 1; l++)
    {
        positions[oldNumVertices + l * 2 + 0] = vertices[l + 0].position;
        positions[oldNumVertices + l * 2 + 1] = vertices[l + 1].position;
    }

    m_vertexBuffer.scheduleUpload(m_vertices);
}

void DebugWireframeInstance::schedule() const
{
    if (m_vertices.empty())
    {
        return;
    }

    if (m_transformDirty)
    {
        m_transformUniform.set(m_transform.matrix());
        m_transformDirty = false;
    }

    m_draw.schedule();
}

DebugSphereInstance::DebugSphereInstance(DebugGeometryRenderer & renderer,
                                         size_t index):
    DebugGeometryInstance(renderer, index)
{
    m_draw = manager().context().createDraw(manager().buildIns().shadedProgram,
                                            gl::GL_TRIANGLES);
    m_draw.addVertexBuffer(manager().buildIns().sphereVertexBuffer);
    m_draw.setIndexBuffer(manager().buildIns().sphereIndexBuffer);
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_colorUniform = m_draw.uniform("Color");
    m_transformUniform = m_draw.uniform("Transform");
}

const glm::vec3 & DebugSphereInstance::color() const
{
    return m_color;
}

float DebugSphereInstance::radius() const
{
    return m_radius;
}

void DebugSphereInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorUniform.set(color);
}

void DebugSphereInstance::setRadius(float radius)
{
    m_radius = radius;
    m_transformDirty = true;
}

void DebugSphereInstance::schedule() const
{
    if (m_transformDirty)
    {
        m_transformUniform.set(m_transform.matrix() * glm::scale(glm::vec3(m_radius)));
        m_transformDirty = false;
    }

    m_draw.schedule();
}

DebugPoseInstance::DebugPoseInstance(DebugGeometryRenderer & renderer,
                                     size_t index):
    DebugGeometryInstance(renderer, index)
{
    m_arrows[0].reset(renderer, std::numeric_limits<size_t>::max());
    m_arrows[0]->setColor({1.0f, 0.0f, 0.0f});

    m_arrows[1].reset(renderer, std::numeric_limits<size_t>::max());
    m_arrows[1]->setColor({0.0f, 1.0f, 0.0f});

    m_arrows[2].reset(renderer, std::numeric_limits<size_t>::max());
    m_arrows[2]->setColor({0.0f, 0.0f, 1.0f});
}

const Pose3D & DebugPoseInstance::pose() const
{
    return m_pose;
}

void DebugPoseInstance::setPose(const Pose3D & pose)
{
    m_pose = pose;
    m_transformDirty = true;
}

void DebugPoseInstance::schedule() const
{
    if (m_transformDirty)
    {
        auto origin = m_pose.position();
        origin = m_transform.pointLocalToWorld(origin);

        auto orientation = m_transform.orientation() * m_pose.orientation();

        m_arrows[0]->reset(origin, orientation * glm::vec3(1.0f, 0.0f, 0.0f));
        m_arrows[1]->reset(origin, orientation * glm::vec3(0.0f, 1.0f, 0.0f));
        m_arrows[2]->reset(origin, orientation * glm::vec3(0.0f, 0.0f, 1.0f));

        m_transformDirty = false;
    }

    for (auto & arrow : m_arrows)
    {
        arrow->schedule();
    }
}

DebugGeometryRenderer::DebugGeometryRenderer(DebugGeometryManager & manager):
    m_manager(manager)
{
    auto globalsLayout = manager.buildIns().shadedProgram.interface().uniformBlock("Globals").layout();
    m_globals = LayoutedBlob(globalsLayout, 1);
    m_globalsBuffer = m_manager.context().createBuffer(globalsLayout);
}

bool DebugGeometryRenderer::visible() const
{
    return m_visible;
}

DebugGeometryManager & DebugGeometryRenderer::manager() const
{
    return m_manager;
}

const Buffer & DebugGeometryRenderer::globalsBuffer() const
{
    return m_globalsBuffer;
}

void DebugGeometryRenderer::setVisible(bool visible)
{
    m_visible = visible;
}

DebugBoxInstance & DebugGeometryRenderer::box(size_t index)
{
    Assert(index < m_boxes.size(), "Invalid index");
    return *m_boxes[index];
}

DebugArrowInstance & DebugGeometryRenderer::arrow(size_t index)
{
    Assert(index < m_arrows.size(), "Invalid index");
    return *m_arrows[index];
}

DebugPointInstance & DebugGeometryRenderer::point(size_t index)
{
    Assert(index < m_points.size(), "Invalid index");
    return *m_points[index];
}

DebugWireframeInstance & DebugGeometryRenderer::wireframe(size_t index)
{
    Assert(index < m_wireframes.size(), "Invalid index");
    return *m_wireframes[index];
}

DebugSphereInstance & DebugGeometryRenderer::sphere(size_t index)
{
    Assert(index < m_spheres.size(), "Invalid index");
    return *m_spheres[index];
}

DebugPoseInstance & DebugGeometryRenderer::pose(size_t index)
{
    Assert(index < m_poses.size(), "Invalid index");
    return *m_poses[index];
}

size_t DebugGeometryRenderer::numBoxes() const
{
    return m_boxes.size();
}

size_t DebugGeometryRenderer::numPoints() const
{
    return m_points.size();
}

size_t DebugGeometryRenderer::numArrows() const
{
    return m_arrows.size();
}

size_t DebugGeometryRenderer::numWireframes() const
{
    return m_wireframes.size();
}

size_t DebugGeometryRenderer::numSpheres() const
{
    return m_spheres.size();
}

size_t DebugGeometryRenderer::numPoses() const
{
    return m_poses.size();
}

void DebugGeometryRenderer::resizeBoxes(size_t count)
{
    if (count < m_boxes.size()) 
    {
        m_boxes.erase(m_boxes.begin() + count, m_boxes.end());
    }
    else
    {
        for (size_t b = m_boxes.size(); b < count; b++)
        {
            addBox({1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.5f}, true);
        }
    }
}

void DebugGeometryRenderer::resizePoints(size_t count)
{
    if (count < m_points.size())
    {
        m_points.erase(m_points.begin() + count, m_points.end());
    }
    else
    {
        for (size_t b = m_points.size(); b < count; b++)
        {
            addPoint({0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.5f});
        }
    }
}

void DebugGeometryRenderer::resizeArrows(size_t count)
{
    if (count < m_arrows.size())
    {
        m_arrows.erase(m_arrows.begin() + count, m_arrows.end());
    }
    else
    {
        for (size_t a = m_arrows.size(); a < count; a++)
        {
            addArrow({}, {}, {1.0f, 0.0f, 0.5f});
        }
    }
}

void DebugGeometryRenderer::resizeWireframes(size_t count)
{
    if (count < m_wireframes.size())
    {
        m_wireframes.erase(m_wireframes.begin() + count, m_wireframes.end());
    }
    else
    {
        for (size_t a = m_wireframes.size(); a < count; a++)
        {
            addWireframe({1.0f, 0.0f, 0.5f});
        }
    }
}

void DebugGeometryRenderer::resizeSpheres(size_t count)
{
    if (count < m_spheres.size())
    {
        m_spheres.erase(m_spheres.begin() + count, m_spheres.end());
    }
    else
    {
        for (size_t s = m_spheres.size(); s < count; s++)
        {
            addSphere({1.0f, 0.0f, 0.5f}, 1.0f);
        }
    }
}

void DebugGeometryRenderer::resizePoses(size_t count)
{
    if (count < m_poses.size())
    {
        m_poses.erase(m_poses.begin() + count, m_poses.end());
    }
    else
    {
        for (size_t s = m_poses.size(); s < count; s++)
        {
            addPose({});
        }
    }
}

DebugBoxInstance & DebugGeometryRenderer::addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe)
{
    m_boxes.push_back(std::make_unique<DebugBoxInstance>(*this, m_boxes.size()));
    auto & box = *m_boxes.back();

    box.setHalfExtent(halfExtent);
    box.setColor(color);
    box.setWireframe(wireframe);

    return box;
}

DebugPointInstance & DebugGeometryRenderer::addPoint(const glm::vec3 & position, const glm::vec3 & color)
{
    m_points.push_back(std::make_unique<DebugPointInstance>(*this, m_points.size()));
    auto & point = *m_points.back();

    point.setTransform(Transform3D::atPosition(position));
    point.setColor(color);

    return point;
}

DebugArrowInstance & DebugGeometryRenderer::addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color)
{
    m_arrows.push_back(std::make_unique<DebugArrowInstance>(*this, m_arrows.size()));
    auto & arrow = *m_arrows.back();

    arrow.reset(origin, delta);
    arrow.setColor(color);

    return arrow;
}

DebugWireframeInstance & DebugGeometryRenderer::addWireframe(const glm::vec3 & color)
{
    m_wireframes.push_back(std::make_unique<DebugWireframeInstance>(*this, m_wireframes.size()));
    auto & wireframe = *m_wireframes.back();

    wireframe.setColor(color);

    return wireframe;
}

DebugSphereInstance & DebugGeometryRenderer::addSphere(const glm::vec3 & color, float radius)
{
    m_spheres.push_back(std::make_unique<DebugSphereInstance>(*this, m_spheres.size()));
    auto & sphere = *m_spheres.back();

    sphere.setRadius(radius);
    sphere.setColor(color);

    return sphere;
}

DebugPoseInstance & DebugGeometryRenderer::addPose(const Pose3D & pose)
{
    m_poses.push_back(std::make_unique<DebugPoseInstance>(*this, m_poses.size()));
    auto & poseInstance = *m_poses.back();

    poseInstance.setPose(pose);

    return poseInstance;
}

void DebugGeometryRenderer::removeBox(size_t index)
{
    Assert(index < m_boxes.size(), "Invalid index");
    m_boxes.erase(m_boxes.begin() + index);
}

void DebugGeometryRenderer::removePoint(size_t index)
{
    Assert(index < m_points.size(), "Invalid index");
    m_points.erase(m_points.begin() + index);
}

void DebugGeometryRenderer::removeArrow(size_t index)
{
    Assert(index < m_arrows.size(), "Invalid index");
    m_arrows.erase(m_arrows.begin() + index);
}

void DebugGeometryRenderer::removeWireframe(size_t index)
{
    Assert(index < m_wireframes.size(), "Invalid index");
    m_wireframes.erase(m_wireframes.begin() + index);
}

void DebugGeometryRenderer::removeSphere(size_t index)
{
    Assert(index < m_spheres.size(), "Invalid index");
    m_spheres.erase(m_spheres.begin() + index);
}

void DebugGeometryRenderer::removePose(size_t index)
{
    Assert(index < m_poses.size(), "Invalid index");
    m_poses.erase(m_poses.begin() + index);
}

void DebugGeometryRenderer::schedule(const Camera3D & camera)
{
    m_globals.field<glm::mat4>("ViewProjection")[0] = camera.viewProjection();
    m_globalsBuffer.scheduleUpload(m_globals);

    for (auto & box : m_boxes)
    {
        box->schedule();
    }

    for (auto & point : m_points)
    {
        point->schedule();
    }

    for (auto & arrow : m_arrows)
    {
        arrow->schedule();
    }

    for (auto & wireframe : m_wireframes)
    {
        wireframe->schedule();
    }

    for (auto & sphere : m_spheres)
    {
        sphere->schedule();
    }

    for (auto & pose : m_poses)
    {
        pose->schedule();
    }
}



















































//bool DebugGeometryInstance::visible() const
//{
//    return m_visible
//}
//
//void DebugGeometryInstance::setVisible(bool visible)
//{
//    m_visible = visible
//}
//
//DebugGeometryInstance::DebugGeometryInstance() = default
//
//DebugBoxInstance::DebugBoxInstance(const Draw & draw,
//                                   const glm::vec3 & halfExtent,
//                                   const glm::vec3 & color):
//    m_draw(draw),
//    m_halfExtent(halfExtent),
//    m_color(color)
//{
//    m_draw.uniform("Color").set(color)
//    m_draw.uniform("Scale").set(halfExtent)
//}
//
//Transform3D & DebugBoxInstance::transform()
//{
//    return m_transform
//}
//
//void DebugBoxInstance::setTransform(const Transform3D & transform)
//{
//    m_transform = transform
//}
//
//Box DebugBoxInstance::toBox() const
//{
//    return Box(m_halfExtent, m_transform)
//}
//
//DebugPointInstance::DebugPointInstance(Program & program,
//                                       const DataLayout & vertexLayout,
//                                       const glm::vec3 & position,
//                                       const glm::vec3 & color):
//    m_position(position),
//    m_color(color)
//{
//    auto & context = program.context()
//    m_draw = context.createDraw(program, gl::GL_POINTS)
//
//    m_draw.uniform("Transform").set(glm::mat4(1.0f))
//    m_draw.uniform("Color").set(color)
//    m_draw.uniform("Scale").set(glm::vec3(1.0f))
//    m_draw.state().rasterizerState().setPointSize(3.0f)
//
//    m_vertexBuffer = context.createBuffer(vertexLayout)
//    m_vertexBuffer.scheduleRawUpload(Blob::fromValue(position), 1)
//
//    m_draw.addVertexBuffer(m_vertexBuffer)
//}
//
//void DebugPointInstance::setPosition(const glm::vec3 & position)
//{
//    if (m_position == position)
//    {
//        return
//    }
//
//    m_vertexBuffer.scheduleRawUpload(Blob::fromValue(position), 1)
//}
//
//
//DebugArrowInstance::DebugArrowInstance(const Program & lineProgram,
//                                       const DataLayout & lineVertexLayout,
//                                       const Draw & coneDraw,
//                                       const glm::vec3 & origin,
//                                       const glm::vec3 & delta,
//                                       const glm::vec3 & color):
//    m_origin(origin),
//    m_delta(delta),
//    m_color(color)
//{
//    auto & context = lineProgram.context()
//
//    /**
//     * Setup line draw
//     */
//    m_lineDraw = context.createDraw(lineProgram, gl::GL_LINES)
//
//    m_lineDraw.uniform("Transform").set(glm::mat4(1.0f))
//    m_lineDraw.uniform("Color").set(color)
//    m_lineDraw.uniform("Scale").set(glm::vec3(1.0f))
//
//    m_lineVertexBuffer = context.createBuffer(lineVertexLayout)
//
//    buildVertices()
//
//    m_lineDraw.addVertexBuffer(m_lineVertexBuffer)
//
//    /**
//     * Setup cone Draw
//     */
//    m_coneDraw = coneDraw
//    setupConeTransform()
//}
//
//void DebugArrowInstance::reset(const glm::vec3 & origin, const glm::vec3 & delta)
//{
//    if (m_origin == origin && m_delta == delta)
//    {
//        return
//    }
//
//    m_origin = origin
//    m_delta = delta
//
//    buildVertices()
//    setupConeTransform()
//}
//
//void DebugArrowInstance::setColor(const glm::vec3 & color)
//{
//    m_lineDraw.uniform("Color").set(color)
//    m_coneDraw.uniform("Color").set(color)
//}
//
//void DebugArrowInstance::buildVertices()
//{
//    LayoutedBlob vertices(m_lineVertexBuffer.layout(), 2)
//    auto positions = vertices.field<glm::vec3>("Position")
//
//    positions[0] = m_origin
//    positions[1] = m_origin + m_delta
//
//    m_lineVertexBuffer.scheduleUpload(vertices)
//}
//
//void DebugArrowInstance::setupConeTransform()
//{
//    if (m_delta == glm::vec3(0.0f))
//    {
//        m_coneDraw.uniform("Transform").set(Transform3D::identity().matrix())
//        return
//    }
//
//    Transform3D transform
//
//    auto rotation = RotationMatrixFromDirectionY(m_delta)
//
//    transform.setPosition(m_origin + m_delta - glm::normalize(m_delta) * ARROW_CONE_HEIGHT)
//    transform.setOrientation(glm::quat_cast(rotation))
//
//    m_coneDraw.uniform("Transform").set(transform.matrix())
//}
//
//DebugWireframeInstance::DebugWireframeInstance(const Program & program,
//                                               const DataLayout & vertexLayout):
//    m_program(program),
//    m_vertexLayout(vertexLayout)
//{
//
//}
//
//void DebugWireframeInstance::setTransform(const Transform3D & transform)
//{
//    m_transform = transform
//    if (m_draw.engaged())
//    {
//        m_draw->uniform("Transform").set(m_transform.matrix())
//    }
//}
//
//void DebugWireframeInstance::addLineStrip(const std::vector<ColoredVertex> & vertices)
//{
//    Assert(vertices.size() >= 2, "Not a line strip")
//
//    m_lineStrips.push_back(vertices)
//    m_lineCount += vertices.size() - 1
//    m_dirty = true
//}
//
//void DebugWireframeInstance::schedule(const Camera3D & camera)
//{
//    if (m_lineStrips.empty())
//    {
//        return
//    }
//
//    if (m_dirty)
//    {
//        m_draw.reset(m_program.context().createDraw(m_program, gl::GL_LINES))
//
//        auto vertices = LayoutedBlob(m_vertexLayout, m_lineCount * 2)
//        auto positions = vertices.field<glm::vec3>("Position")
//        auto colors = vertices.field<glm::vec3>("Color")
//
//        u32 i = 0
//
//        for (auto & strip : m_lineStrips)
//        {
//            for (u32 v = 0 v < strip.size() - 1 v++)
//            {
//                positions[i + 0] = strip[v].position
//                positions[i + 1] = strip[v + 1].position
//                colors[i + 0] = strip[v].color
//                colors[i + 1] = strip[v + 1].color
//
//                i += 2
//            }
//        }
//
//        m_draw->addVertices(vertices)
//        m_draw->uniform("Transform").set(m_transform.matrix())
//
//        m_dirty = false
//    }
//
//    m_draw->uniform("ViewProjection").set(camera.viewProjection())
//    m_draw->schedule()
//}
//
//DebugSphereInstance::DebugSphereInstance(const Program & program,
//                                         const Buffer & vertexBuffer,
//                                         const Buffer & indexBuffer,
//                                         const glm::vec3 & color,
//                                         const float scale):
//    m_program(program),
//    m_vertexBuffer(vertexBuffer),
//    m_indexBuffer(indexBuffer),
//    m_color(color),
//    m_scale(scale)
//{
//
//}
//
//void DebugSphereInstance::setTransform(const Transform3D & transform)
//{
//    m_transform = transform
//    if (m_draw.engaged())
//    {
//        m_draw->uniform("Transform").set(m_transform.scaled(m_scale).matrix())
//    }
//}
//
//void DebugSphereInstance::schedule(const Camera3D & camera)
//{
//    if (m_dirty)
//    {
//        m_draw.reset(m_program.context().createDraw(m_program, gl::GL_TRIANGLES))
//
//        m_draw->addVertexBuffer(m_vertexBuffer)
//        m_draw->setIndexBuffer(m_indexBuffer)
//        m_draw->uniform("Color").set(m_color)
//        m_draw->uniform("Transform").set(m_transform.scaled(m_scale).matrix())
//
//        m_dirty = false
//    }
//
//    m_draw->uniform("ViewProjection").set(camera.viewProjection())
//    m_draw->schedule()
//}
//
//DebugGeometryRenderer::DebugGeometryRenderer(Context & context, const Camera3D & camera):
//    m_context(context),
//    m_camera(camera)
//{
//
//
//}
//
//
//bool DebugGeometryRenderer::visible() const
//{
//    return m_visible
//}
//
//void DebugGeometryRenderer::setVisible(bool visible)
//{
//    m_visible = visible
//}
//
//DebugBoxInstance & DebugGeometryRenderer::box(size_t index)
//{
//    Assert(index < m_boxes.size(), " ")
//    return m_boxes[index]
//}
//
//DebugArrowInstance & DebugGeometryRenderer::arrow(size_t index)
//{
//    Assert(index < m_arrows.size(), " ")
//    return m_arrows[index]
//}
//
//DebugPointInstance & DebugGeometryRenderer::point(size_t index)
//{
//    Assert(index < m_points.size(), " ")
//    return m_points[index]
//}
//
//DebugWireframeInstance & DebugGeometryRenderer::wireframe(size_t index)
//{
//    Assert(index < m_wireframes.size(), " ")
//    return m_wireframes[index]
//}
//
//DebugSphereInstance & DebugGeometryRenderer::sphere(size_t index)
//{
//    Assert(index < m_spheres.size(), " ")
//    return m_spheres[index]
//}
//
//void DebugGeometryRenderer::resizeArrows(size_t count, const glm::vec3 & color, bool visible)
//{
//    if (count > m_arrows.size())
//    {
//        allocateArrows(count - m_arrows.size(), color, visible)
//    }
//    else
//    {
//        m_arrows.erase(m_arrows.begin() + count, m_arrows.end())
//        for (auto & arrow : m_arrows)
//        {
//            // TODO: Set color
//            arrow.setVisible(visible)
//        }
//    }
//}
//
////void DebugGeometryRenderer::allocateBoxes(size_t count, bool wireframe)
////{
////
////}
//
//void DebugGeometryRenderer::allocatePoints(size_t count, const glm::vec3 & color, bool visible)
//{
//    for (uint p = 0 p < count p++)
//    {
//        auto index = addPoint({}, color)
//        point(index).setVisible(visible)
//    }
//}
//
//void DebugGeometryRenderer::allocateArrows(size_t count, const glm::vec3 & color, bool visible)
//{
//    for (uint a = 0 a < count a++)
//    {
//        auto index = addArrow({}, {}, color)
//        arrow(index).setVisible(visible)
//    }
//}
////void DebugGeometryRenderer::allocateArrows(size_t count)
////{
//
////}
//
//
//size_t DebugGeometryRenderer::addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe)
//{
//    Draw draw
//
//    if (wireframe)
//    {
//        draw = m_context.createDraw(m_unicolorProgram, gl::GL_LINES)
//        draw.addVertexBuffer(m_boxLinesVertexBuffer)
//        draw.setIndexBuffer(m_boxLinesIndexBuffer)
//    }
//    else
//    {
//        draw = m_context.createDraw(m_shadedProgram, gl::GL_TRIANGLES)
//        draw.addVertexBuffer(m_boxTrianglesVertexBuffer)
//        draw.setIndexBuffer(m_boxTrianglesIndexBuffer)
//    }
//
//    draw.state().rasterizerState().setLineWidth(2.0f)
//
//    m_boxes.push_back({draw, halfExtent, color})
//
//    return m_boxes.size() - 1
//}
//
//size_t DebugGeometryRenderer::addPoint(const glm::vec3 & position, const glm::vec3 & color)
//{
//    m_points.push_back({m_unicolorProgram, m_unicolorDataLayout, position, color})
//    return m_points.size() - 1
//}
//
//size_t DebugGeometryRenderer::addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color)
//{
//    auto draw = m_context.createDraw(m_shadedProgram, gl::GL_TRIANGLES)
//    draw.addVertexBuffer(m_coneVertexBuffer)
//    draw.setIndexBuffer(m_coneIndexBuffer)
//    draw.uniform("Color").set(color)
//    draw.uniform("Scale").set(glm::vec3(1.0f, 1.0f, 1.0f))
//
//    m_arrows.push_back({m_unicolorProgram, m_unicolorDataLayout, draw, origin, delta, color})
//    return m_arrows.size() - 1
//}
//
//size_t DebugGeometryRenderer::addWireframe()
//{
//    m_wireframes.emplace_back(m_vertexColorProgram, m_vertexColorDataLayout)
//    return m_wireframes.size() - 1
//}
//
//size_t DebugGeometryRenderer::addSphere(const glm::vec3 & color, float radius)
//{
//    m_spheres.emplace_back(m_shadedProgram, m_sphereVertexBuffer, m_sphereIndexBuffer, color, radius)
//    return m_spheres.size() - 1
//
//}
//
//DebugBoxInstance & DebugGeometryRenderer::addAndGetBox(const glm::vec3 & halfExtent,
//                                                       const glm::vec3 & color,
//                                                       bool wireframe)
//{
//    return box(addBox(halfExtent, color, wireframe))
//}
//
//DebugWireframeInstance & DebugGeometryRenderer::addAndGetWireframe()
//{
//    return wireframe(addWireframe())
//}
//
//DebugSphereInstance & DebugGeometryRenderer::addAndGetSphere(const glm::vec3 & color, float radius)
//{
//    return sphere(addSphere(color, radius))
//}
//
//void DebugGeometryRenderer::removeBox(size_t index)
//{
//    Assert(index < m_boxes.size(), " ")
//    m_boxes.erase(m_boxes.begin() + index)
//}
//
//void DebugGeometryRenderer::removePoint(size_t index)
//{
//    Assert(index < m_points.size(), " ")
//    m_points.erase(m_points.begin() + index)
//}
//
//void DebugGeometryRenderer::removeArrow(size_t index)
//{
//    Assert(index < m_arrows.size(), " ")
//    m_arrows.erase(m_arrows.begin() + index)
//}
//
//void DebugGeometryRenderer::schedule()
//{
//    if (!m_visible)
//    {
//        return
//    }
//
//    for (auto & box : m_boxes)
//    {
//        if (!box.visible())
//        {
//            continue
//        }
//
//        auto & draw = box.m_draw
//
//        draw.uniform("Transform").set(box.m_transform.matrix())
//        draw.uniform("ViewProjection").set(m_camera.viewProjection())
//
//        draw.schedule()
//    }
//
//    for (auto & point : m_points)
//    {
//        if (!point.visible())
//        {
//            continue
//        }
//        auto & draw = point.m_draw
//        draw.uniform("ViewProjection").set(m_camera.viewProjection())
//        draw.schedule()
//    }
//
//    for (auto & arrow : m_arrows)
//    {
//        if (!arrow.visible())
//        {
//            continue
//        }
//
//        auto & lineDraw = arrow.m_lineDraw
//        lineDraw.uniform("ViewProjection").set(m_camera.viewProjection())
//        lineDraw.schedule()
//
//        auto & coneDraw = arrow.m_coneDraw
//        coneDraw.uniform("ViewProjection").set(m_camera.viewProjection())
//        coneDraw.schedule()
//    }
//
//    for (auto & wireframe : m_wireframes)
//    {
//        wireframe.schedule(m_camera)
//    }
//
//    for (auto & sphere : m_spheres)
//    {
//        sphere.schedule(m_camera)
//    }
//}

}
