#include <Deliberation/Scene/Debug/DebugGeometryNode.h>

#include <iostream>
#include <memory>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation
{
DebugGeometryInstance::DebugGeometryInstance(DebugGeometryNode & renderer)
    : m_renderer(renderer)
{
}

size_t DebugGeometryInstance::index() const { return m_index; }

DebugGeometryNode & DebugGeometryInstance::renderer() const
{
    return m_renderer;
}

DebugGeometryRenderer & DebugGeometryInstance::manager() const
{
    return m_renderer.manager();
}

const DebugGeometryRenderer::BuildIns & DebugGeometryInstance::buildIns() const
{
    return manager().buildIns();
}

bool DebugGeometryInstance::visible() const { return m_visible; }

const Transform3D & DebugGeometryInstance::transform() const
{
    return m_transform;
}

void DebugGeometryInstance::setIndex(size_t index) { m_index = index; }

void DebugGeometryInstance::setVisible(bool visible) { m_visible = visible; }

void DebugGeometryInstance::setTransform(const Transform3D & transform)
{
    m_transform = transform;
    m_transformDirty = true;
}

DebugBoxInstance::DebugBoxInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
}

bool DebugBoxInstance::wireframe() const { return m_wireframe; }

const glm::vec3 & DebugBoxInstance::halfExtent() const { return m_halfExtent; }

const glm::vec3 & DebugBoxInstance::color() const { return m_color; }

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

void DebugBoxInstance::render() const
{
    if (m_meshDirty)
    {
        if (m_wireframe)
        {
            m_draw = GetGlobal<DrawContext>()->createDraw(
                buildIns().unicolorProgram, DrawPrimitive::Lines);
            m_draw.addVertexBuffer(buildIns().boxLinesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxLinesIndexBuffer);
        }
        else
        {
            m_draw =
                GetGlobal<DrawContext>()->createDraw(buildIns().shadedProgram);
            m_draw.addVertexBuffer(buildIns().boxTrianglesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxTrianglesIndexBuffer);
        }

        m_draw.setState(m_renderer.drawState());
        if (m_wireframe)
        {
            m_draw.state().rasterizerState().setPrimitive(DrawPrimitive::Lines);
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

    m_draw.render();
}

Box DebugBoxInstance::toBox() const { return Box(m_halfExtent, m_transform); }

DebugPointInstance::DebugPointInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
    m_draw = GetGlobal<DrawContext>()->createDraw(
        manager().buildIns().unicolorProgram, DrawPrimitive::Points);
    m_draw.addVertexBuffer(manager().buildIns().pointVertexBuffer);
    ;
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_draw.setState(m_renderer.drawState());
    m_draw.state().rasterizerState().setPointSize(3.0f);
    m_transformUniform = m_draw.uniform("Transform");

    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 & DebugPointInstance::color() const { return m_color; }

void DebugPointInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugPointInstance::render() const
{
    if (m_transformDirty)
    {
        m_transformUniform.set(m_transform.matrix());
        m_transformDirty = false;
    }

    m_draw.render();
}

DebugArrowInstance::DebugArrowInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
    m_lineVertices = LayoutedBlob(DataLayout("Position", Type_Vec3), 2);
    m_lineVertexBuffer =
        GetGlobal<DrawContext>()->createBuffer(m_lineVertices.layout());

    m_lineDraw = GetGlobal<DrawContext>()->createDraw(
        manager().buildIns().unicolorProgram,
        DrawPrimitive::Lines,
        "DebugLineDraw");
    m_lineDraw.addVertexBuffer(m_lineVertexBuffer);
    m_lineDraw.setState(m_renderer.drawState());
    m_lineDraw.state().rasterizerState().setPrimitive(DrawPrimitive::Lines);
    m_lineDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_lineDraw.uniform("Transform").set(glm::mat4(1.0f));
    m_lineColorUniform = m_lineDraw.uniform("Color");

    m_coneDraw = GetGlobal<DrawContext>()->createDraw(
        manager().buildIns().shadedProgram,
        DrawPrimitive::Triangles,
        "DebugConeDraw");
    m_coneDraw.setState(m_renderer.drawState());
    m_coneDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_coneDraw.addVertexBuffer(manager().buildIns().coneVertexBuffer);
    m_coneDraw.setIndexBuffer(manager().buildIns().coneIndexBuffer);
    m_coneColorUniform = m_coneDraw.uniform("Color");
    m_coneTransformUniform = m_coneDraw.uniform("Transform");
}

const glm::vec3 & DebugArrowInstance::origin() const { return m_origin; }

const glm::vec3 & DebugArrowInstance::delta() const { return m_delta; }

const glm::vec3 & DebugArrowInstance::color() const { return m_color; }

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

void DebugArrowInstance::reset(
    const glm::vec3 & origin, const glm::vec3 & delta)
{
    m_origin = origin;
    m_delta = delta;

    auto positions = m_lineVertices.field<glm::vec3>("Position");
    positions[0] = m_origin;
    positions[1] = m_origin + m_delta;

    m_lineVertexBuffer.upload(m_lineVertices);

    if (glm::length2(m_delta) < 0.001f)
    {
        m_coneTransformUniform.set(Transform3D::identity().matrix());
        return;
    }

    Transform3D transform;

    auto rotation = RotationMatrixFromDirectionY(m_delta);

    transform.setPosition(
        m_origin + m_delta -
        glm::normalize(m_delta) * DebugGeometryRenderer::ARROW_CONE_HEIGHT);
    transform.setOrientation(glm::quat_cast(rotation));

    m_coneTransformUniform.set(transform.matrix());
}

void DebugArrowInstance::render() const
{
    m_lineDraw.render();
    m_coneDraw.render();
}

DebugWireframeInstance::DebugWireframeInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
    m_vertices = LayoutedBlob(DataLayout("Position", Type_Vec3));

    m_vertexBuffer = GetGlobal<DrawContext>()->createBuffer(m_vertices.layout());

    m_draw = GetGlobal<DrawContext>()->createDraw(
        manager().buildIns().unicolorProgram, DrawPrimitive::Lines);
    m_draw.setState(m_renderer.drawState());
    m_draw.state().rasterizerState().setPrimitive(DrawPrimitive::Lines);
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_transformUniform = m_draw.uniform("Transform");
    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 & DebugWireframeInstance::color() const { return m_color; }

void DebugWireframeInstance::setColor(const glm::vec3 & color)
{
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugWireframeInstance::addLineStrip(
    const std::vector<BasicVertex> & vertices)
{
    AssertM(vertices.size() >= 2, "Not a line strip");

    auto oldNumVertices = m_vertices.count();

    m_vertices.resize(oldNumVertices + (vertices.size() - 1) * 2);

    auto positions = m_vertices.field<glm::vec3>("Position");

    for (uint l = 0; l < vertices.size() - 1; l++)
    {
        positions[oldNumVertices + l * 2 + 0] = vertices[l + 0].position;
        positions[oldNumVertices + l * 2 + 1] = vertices[l + 1].position;
    }

    m_vertexBuffer.upload(m_vertices);
}

void DebugWireframeInstance::render() const
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

    m_draw.render();
}

DebugSphereInstance::DebugSphereInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
    m_draw =
        GetGlobal<DrawContext>()->createDraw(manager().buildIns().shadedProgram);
    m_draw.addVertexBuffer(manager().buildIns().sphereVertexBuffer);
    m_draw.setIndexBuffer(manager().buildIns().sphereIndexBuffer);
    m_draw.setState(m_renderer.drawState());
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_colorUniform = m_draw.uniform("Color");
    m_transformUniform = m_draw.uniform("Transform");
}

const glm::vec3 & DebugSphereInstance::color() const { return m_color; }

float DebugSphereInstance::radius() const { return m_radius; }

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

void DebugSphereInstance::render() const
{
    if (m_transformDirty)
    {
        m_transformUniform.set(
            m_transform.matrix() * glm::scale(glm::vec3(m_radius)));
        m_transformDirty = false;
    }

    m_draw.render();
    m_draw.render();
}

DebugPoseInstance::DebugPoseInstance(DebugGeometryNode & renderer)
    : DebugGeometryInstance(renderer)
{
    m_arrows[0].emplace(renderer);
    m_arrows[0]->setColor({1.0f, 0.0f, 0.0f});

    m_arrows[1].emplace(renderer);
    m_arrows[1]->setColor({0.0f, 1.0f, 0.0f});

    m_arrows[2].emplace(renderer);
    m_arrows[2]->setColor({0.0f, 0.0f, 1.0f});
}

const Pose3D & DebugPoseInstance::pose() const { return m_pose; }

void DebugPoseInstance::setPose(const Pose3D & pose)
{
    m_pose = pose;
    m_transformDirty = true;
}

void DebugPoseInstance::render() const
{
    if (m_transformDirty)
    {
        auto origin = m_pose.position();
        origin = m_transform.pointLocalToWorld(origin);

        auto orientation = m_transform.orientation() * m_pose.orientation();

        m_arrows[0]->reset(
            origin, orientation * glm::vec3(m_transform.scale(), 0.0f, 0.0f));
        m_arrows[1]->reset(
            origin, orientation * glm::vec3(0.0f, m_transform.scale(), 0.0f));
        m_arrows[2]->reset(
            origin, orientation * glm::vec3(0.0f, 0.0f, m_transform.scale()));

        m_transformDirty = false;
    }

    for (auto & arrow : m_arrows)
    {
        arrow->render();
    }
}

DebugGeometryNode::DebugGeometryNode(DebugGeometryRenderer & manager)
    : m_manager(manager)
{
    auto globalsLayout = manager.buildIns()
                             .shadedProgram.interface()
                             .uniformBlockRef("Globals")
                             .layout();
    m_globals = LayoutedBlob(globalsLayout, 1);
    m_globalsBuffer = GetGlobal<DrawContext>()->createBuffer(globalsLayout);
}

bool DebugGeometryNode::visible() const { return m_visible; }

DebugGeometryRenderer & DebugGeometryNode::manager() const { return m_manager; }

const Buffer & DebugGeometryNode::globalsBuffer() const
{
    return m_globalsBuffer;
}

void DebugGeometryNode::setVisible(bool visible) { m_visible = visible; }

void DebugGeometryNode::setDrawState(const DrawState & drawState)
{
    m_drawState = drawState;
}

DebugBoxInstance & DebugGeometryNode::addBox(
    const glm::vec3 & halfExtent,
    const glm::vec3 & color,
    bool              wireframe,
    size_t            index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_boxStreamingIndex++;

    auto ptr = std::make_unique<DebugBoxInstance>(*this);

    if (index == NO_INDEX)
        index = m_boxes.emplace(std::move(ptr));
    else
        m_boxes.replace_at(index, std::move(ptr));

    auto & box = *m_boxes[index];

    box.setHalfExtent(halfExtent);
    box.setColor(color);
    box.setWireframe(wireframe);
    box.setIndex(index);

    return box;
}

DebugPointInstance & DebugGeometryNode::addPoint(
    const glm::vec3 & position, const glm::vec3 & color, size_t index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_pointStreamingIndex++;

    auto ptr = std::make_unique<DebugPointInstance>(*this);

    if (index == NO_INDEX)
        index = m_points.emplace(std::move(ptr));
    else
        m_points.replace_at(index, std::move(ptr));

    auto & point = *m_points[index];

    point.setTransform(Transform3D::atPosition(position));
    point.setColor(color);
    point.setIndex(index);

    return point;
}

DebugArrowInstance & DebugGeometryNode::addArrow(
    const glm::vec3 & origin,
    const glm::vec3 & delta,
    const glm::vec3 & color,
    size_t            index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_arrowStreamingIndex++;

    auto ptr = std::make_unique<DebugArrowInstance>(*this);

    if (index == NO_INDEX)
        index = m_arrows.emplace(std::move(ptr));
    else
        m_arrows.replace_at(index, std::move(ptr));

    auto & arrow = *m_arrows[index];

    arrow.reset(origin, delta);
    arrow.setColor(color);
    arrow.setIndex(index);

    return arrow;
}

DebugWireframeInstance &
DebugGeometryNode::addWireframe(const glm::vec3 & color, size_t index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_wireframeStreamingIndex++;

    auto ptr = std::make_unique<DebugWireframeInstance>(*this);

    if (index == NO_INDEX)
        index = m_wireframes.emplace(std::move(ptr));
    else
        m_wireframes.replace_at(index, std::move(ptr));

    auto & wireframe = *m_wireframes[index];

    wireframe.setColor(color);
    wireframe.setIndex(index);

    return wireframe;
}

DebugSphereInstance & DebugGeometryNode::addSphere(
    const glm::vec3 & color, float radius, size_t index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_sphereStreamingIndex++;

    auto ptr = std::make_unique<DebugSphereInstance>(*this);

    if (index == NO_INDEX)
        index = m_spheres.emplace(std::move(ptr));
    else
        m_spheres.replace_at(index, std::move(ptr));

    auto & sphere = *m_spheres[index];

    sphere.setRadius(radius);
    sphere.setColor(color);
    sphere.setIndex(index);

    return sphere;
}

DebugPoseInstance &
DebugGeometryNode::addPose(const Pose3D & pose, size_t index)
{
    AssertM(index == NO_INDEX || !m_streaming, "Can't stream with explicit indices");

    if (m_streaming) index = m_poseStreamingIndex++;

    auto ptr = std::make_unique<DebugPoseInstance>(*this);

    if (index == NO_INDEX)
        index = m_poses.emplace(std::move(ptr));
    else
        m_poses.replace_at(index, std::move(ptr));

    auto & poseInstance = *m_poses[index];

    poseInstance.setPose(pose);
    poseInstance.setIndex(index);

    return poseInstance;
}

DebugBoxInstance & DebugGeometryNode::box(size_t index)
{
    if (m_boxes.contains(index)) return *m_boxes[index];
    return addBox({}, {}, false, index);
}

DebugPointInstance & DebugGeometryNode::point(size_t index)
{
    if (m_points.contains(index)) return *m_points[index];
    return addPoint({}, {}, index);
}

DebugArrowInstance & DebugGeometryNode::arrow(size_t index)
{
    if (m_arrows.contains(index)) return *m_arrows[index];
    return addArrow({}, {}, {}, index);
}

DebugWireframeInstance & DebugGeometryNode::wireframe(size_t index)
{
    if (m_wireframes.contains(index)) return *m_wireframes[index];
    return addWireframe({}, index);
}

DebugSphereInstance & DebugGeometryNode::sphere(size_t index)
{
    if (m_spheres.contains(index)) return *m_spheres[index];
    return addSphere({}, {}, index);
}

DebugPoseInstance & DebugGeometryNode::pose(size_t index)
{
    if (m_poses.contains(index)) return *m_poses[index];
    return addPose({}, index);
}

void DebugGeometryNode::removeBox(size_t index)
{
    AssertM(m_boxes.contains(index), "Invalid index");
    m_boxes.erase(index);
}

void DebugGeometryNode::removePoint(size_t index)
{
    AssertM(m_points.contains(index), "Invalid index");
    m_points.erase(index);
}

void DebugGeometryNode::removeArrow(size_t index)
{
    AssertM(m_arrows.contains(index), "Invalid index");
    m_arrows.erase(index);
}

void DebugGeometryNode::removeWireframe(size_t index)
{
    AssertM(m_wireframes.contains(index), "Invalid index");
    m_wireframes.erase(index);
}

void DebugGeometryNode::removeSphere(size_t index)
{
    AssertM(m_spheres.contains(index), "Invalid index");
    m_spheres.erase(index);
}

void DebugGeometryNode::removePose(size_t index)
{
    AssertM(m_poses.contains(index), "Invalid index");
    m_poses.erase(index);
}

void DebugGeometryNode::beginPrimitives()
{
    m_streaming = true;
    m_boxStreamingIndex = 0;
    m_arrowStreamingIndex = 0;
    m_pointStreamingIndex = 0;
    m_wireframeStreamingIndex = 0;
    m_sphereStreamingIndex = 0;
    m_poseStreamingIndex = 0;
}

void DebugGeometryNode::endPrimitives()
{
    hideRemainingPrimitives(m_boxes, m_boxStreamingIndex);
    hideRemainingPrimitives(m_arrows, m_arrowStreamingIndex);
    hideRemainingPrimitives(m_points, m_pointStreamingIndex);
    hideRemainingPrimitives(m_wireframes, m_wireframeStreamingIndex);
    hideRemainingPrimitives(m_spheres, m_sphereStreamingIndex);
    hideRemainingPrimitives(m_poses, m_poseStreamingIndex);

    m_streaming = false;
}

void DebugGeometryNode::render(const Camera3D & camera)
{
    if (!m_visible) return;

    m_globals.field<glm::mat4>("ViewProjection")[0] = camera.viewProjection();
    m_globalsBuffer.upload(m_globals);

    for (auto & box : m_boxes)
        if (box->visible()) box->render();
    for (auto & point : m_points)
        if (point->visible()) point->render();
    for (auto & arrow : m_arrows)
        if (arrow->visible()) arrow->render();
    for (auto & wireframe : m_wireframes)
        if (wireframe->visible()) wireframe->render();
    for (auto & sphere : m_spheres)
        if (sphere->visible()) sphere->render();
    for (auto & pose : m_poses)
        if (pose->visible()) pose->render();
}

template<typename T>
void DebugGeometryNode::hideRemainingPrimitives(SparseVector<std::unique_ptr<T>> & primitives, size_t begin)
{
    for (size_t i = begin; i < primitives.capacity(); i++) if (primitives.contains(i)) primitives[i]->setVisible(false);
}
}
