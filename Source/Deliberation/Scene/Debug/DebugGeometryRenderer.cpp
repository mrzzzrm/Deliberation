#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include <iostream>
#include <memory>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/DataLayout.h>
#include <Deliberation/Core/LayoutedBlob.h>
#include <Deliberation/Core/Types.h>

#include <Deliberation/Draw/DrawContext.h>
#include <Deliberation/Draw/ProgramInterface.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/ConeMesh.h>
#include <Deliberation/Scene/CuboidMesh.h>
#include <Deliberation/Scene/MeshCompiler.h>
#include <Deliberation/Scene/UVSphere.h>

namespace deliberation {

DebugGeometryInstance::DebugGeometryInstance(DebugGeometryRenderer &renderer) :
    m_renderer(renderer) {}

size_t DebugGeometryInstance::index() const {
    return m_index;
}

DebugGeometryRenderer &DebugGeometryInstance::renderer() const {
    return m_renderer;
}

DebugGeometryManager &DebugGeometryInstance::manager() const {
    return m_renderer.manager();
}

const DebugGeometryManager::BuildIns &DebugGeometryInstance::buildIns() const {
    return manager().buildIns();
}

bool DebugGeometryInstance::visible() const {
    return m_visible;
}

const Transform3D &DebugGeometryInstance::transform() const {
    return m_transform;
}

void DebugGeometryInstance::setIndex(size_t index) {
    m_index = index;
}

void DebugGeometryInstance::setVisible(bool visible) {
    m_visible = visible;
}

void DebugGeometryInstance::setTransform(const Transform3D &transform) {
    m_transform = transform;
    m_transformDirty = true;
}

DebugBoxInstance::DebugBoxInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {}

bool DebugBoxInstance::wireframe() const {
    return m_wireframe;
}

const glm::vec3 &DebugBoxInstance::halfExtent() const {
    return m_halfExtent;
}

const glm::vec3 &DebugBoxInstance::color() const {
    return m_color;
}

void DebugBoxInstance::setHalfExtent(const glm::vec3 &halfExtent) {
    m_halfExtent = halfExtent;
    m_meshDirty = true;
}

void DebugBoxInstance::setColor(const glm::vec3 &color) {
    m_color = color;
    m_colorDirty = true;
}

void DebugBoxInstance::setWireframe(bool wireframe) {
    m_wireframe = wireframe;
    m_meshDirty = true;
}

void DebugBoxInstance::render() const {
    if (m_meshDirty) {
        if (m_wireframe) {
            m_draw = manager().drawContext().createDraw(
                buildIns().unicolorProgram,
                DrawPrimitive::Lines
            );
            m_draw.addVertexBuffer(buildIns().boxLinesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxLinesIndexBuffer);
        } else {
            m_draw = manager().drawContext().createDraw(
                buildIns().shadedProgram);
            m_draw.addVertexBuffer(buildIns().boxTrianglesVertexBuffer);
            m_draw.setIndexBuffer(buildIns().boxTrianglesIndexBuffer);
        }

        m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

        m_transformUniform = m_draw.uniform("Transform");

        m_meshDirty = false;
        m_colorDirty = true;
    }

    if (m_colorDirty) {
        m_draw.uniform("Color").set(m_color);
        m_colorDirty = false;
    }

    if (m_transformDirty) {
        m_transformUniform.set(m_transform.matrix() * glm::scale(m_halfExtent));
        m_transformDirty = false;
    }

    m_draw.render();
}

Box DebugBoxInstance::toBox() const {
    return Box(m_halfExtent, m_transform);
}

DebugPointInstance::DebugPointInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {
    m_draw = manager().drawContext().createDraw(manager().buildIns().unicolorProgram, DrawPrimitive::Points);
    m_draw.addVertexBuffer(manager().buildIns().pointVertexBuffer);;
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_draw.state().rasterizerState().setPointSize(3.0f);
    m_transformUniform = m_draw.uniform("Transform");

    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 &DebugPointInstance::color() const {
    return m_color;
}

void DebugPointInstance::setColor(const glm::vec3 &color) {
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugPointInstance::render() const {
    if (m_transformDirty) {
        m_transformUniform.set(m_transform.matrix());
        m_transformDirty = false;
    }

    m_draw.render();
}

DebugArrowInstance::DebugArrowInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {
    m_lineVertices = LayoutedBlob(DataLayout("Position", Type_Vec3), 2);
    m_lineVertexBuffer = manager().drawContext().createBuffer(m_lineVertices.layout());

    m_lineDraw = manager().drawContext().createDraw(manager().buildIns().unicolorProgram,
                                                DrawPrimitive::Lines);
    m_lineDraw.addVertexBuffer(m_lineVertexBuffer);
    m_lineDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_lineDraw.uniform("Transform").set(glm::mat4(1.0f));
    m_lineColorUniform = m_lineDraw.uniform("Color");

    m_coneDraw = manager().drawContext().createDraw(manager().buildIns().shadedProgram);
    m_coneDraw.setUniformBuffer("Globals", m_renderer.globalsBuffer());
    m_coneDraw.addVertexBuffer(manager().buildIns().coneVertexBuffer);
    m_coneDraw.setIndexBuffer(manager().buildIns().coneIndexBuffer);
    m_coneColorUniform = m_coneDraw.uniform("Color");
    m_coneTransformUniform = m_coneDraw.uniform("Transform");
}

const glm::vec3 &DebugArrowInstance::origin() const {
    return m_origin;
}

const glm::vec3 &DebugArrowInstance::delta() const {
    return m_delta;
}

const glm::vec3 &DebugArrowInstance::color() const {
    return m_color;
}

void DebugArrowInstance::setOrigin(const glm::vec3 &origin) {
    reset(origin, m_delta);
}

void DebugArrowInstance::setDelta(const glm::vec3 &delta) {
    reset(m_origin, delta);
}

void DebugArrowInstance::setColor(const glm::vec3 &color) {
    m_color = color;
    m_lineColorUniform.set(m_color);
    m_coneColorUniform.set(m_color);
}

void DebugArrowInstance::reset(const glm::vec3 &origin, const glm::vec3 &delta) {
    m_origin = origin;
    m_delta = delta;

    auto positions = m_lineVertices.field<glm::vec3>("Position");
    positions[0] = m_origin;
    positions[1] = m_origin + m_delta;

    m_lineVertexBuffer.upload(m_lineVertices);

    if (glm::length2(m_delta) < 0.001f) {
        m_coneTransformUniform.set(Transform3D::identity().matrix());
        return;
    }

    Transform3D transform;

    auto rotation = RotationMatrixFromDirectionY(m_delta);

    transform.setPosition(m_origin + m_delta - glm::normalize(m_delta) * DebugGeometryManager::ARROW_CONE_HEIGHT);
    transform.setOrientation(glm::quat_cast(rotation));

    m_coneTransformUniform.set(transform.matrix());
}

void DebugArrowInstance::render() const {
    m_lineDraw.render();
    m_coneDraw.render();
}

DebugWireframeInstance::DebugWireframeInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {
    m_vertices = LayoutedBlob(DataLayout("Position", Type_Vec3));

    m_vertexBuffer = manager().drawContext().createBuffer(m_vertices.layout());

    m_draw = manager().drawContext().createDraw(manager().buildIns().unicolorProgram, DrawPrimitive::Lines);
    m_draw.addVertexBuffer(m_vertexBuffer);
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_transformUniform = m_draw.uniform("Transform");
    m_colorUniform = m_draw.uniform("Color");
}

const glm::vec3 &DebugWireframeInstance::color() const {
    return m_color;
}

void DebugWireframeInstance::setColor(const glm::vec3 &color) {
    m_color = color;
    m_colorUniform.set(m_color);
}

void DebugWireframeInstance::addLineStrip(const std::vector<BasicVertex> &vertices) {
    Assert(vertices.size() >= 2, "Not a line strip");

    auto oldNumVertices = m_vertices.count();

    m_vertices.resize(oldNumVertices + (vertices.size() - 1) * 2);

    auto positions = m_vertices.field<glm::vec3>("Position");

    for (uint l = 0; l < vertices.size() - 1; l++) {
        positions[oldNumVertices + l * 2 + 0] = vertices[l + 0].position;
        positions[oldNumVertices + l * 2 + 1] = vertices[l + 1].position;
    }

    m_vertexBuffer.upload(m_vertices);
}

void DebugWireframeInstance::render() const {
    if (m_vertices.empty()) {
        return;
    }

    if (m_transformDirty) {
        m_transformUniform.set(m_transform.matrix());
        m_transformDirty = false;
    }

    m_draw.render();
}

DebugSphereInstance::DebugSphereInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {
    m_draw = manager().drawContext().createDraw(manager().buildIns().shadedProgram);
    m_draw.addVertexBuffer(manager().buildIns().sphereVertexBuffer);
    m_draw.setIndexBuffer(manager().buildIns().sphereIndexBuffer);
    m_draw.setUniformBuffer("Globals", m_renderer.globalsBuffer());

    m_colorUniform = m_draw.uniform("Color");
    m_transformUniform = m_draw.uniform("Transform");
}

const glm::vec3 &DebugSphereInstance::color() const {
    return m_color;
}

float DebugSphereInstance::radius() const {
    return m_radius;
}

void DebugSphereInstance::setColor(const glm::vec3 &color) {
    m_color = color;
    m_colorUniform.set(color);
}

void DebugSphereInstance::setRadius(float radius) {
    m_radius = radius;
    m_transformDirty = true;
}

void DebugSphereInstance::render() const {
    if (m_transformDirty) {
        m_transformUniform.set(m_transform.matrix() * glm::scale(glm::vec3(m_radius)));
        m_transformDirty = false;
    }

    m_draw.render();
}

DebugPoseInstance::DebugPoseInstance(DebugGeometryRenderer &renderer) :
    DebugGeometryInstance(renderer) {
    m_arrows[0].emplace(renderer);
    m_arrows[0]->setColor({1.0f, 0.0f, 0.0f});

    m_arrows[1].emplace(renderer);
    m_arrows[1]->setColor({0.0f, 1.0f, 0.0f});

    m_arrows[2].emplace(renderer);
    m_arrows[2]->setColor({0.0f, 0.0f, 1.0f});
}

const Pose3D &DebugPoseInstance::pose() const {
    return m_pose;
}

void DebugPoseInstance::setPose(const Pose3D &pose) {
    m_pose = pose;
    m_transformDirty = true;
}

void DebugPoseInstance::render() const {
    if (m_transformDirty) {
        auto origin = m_pose.position();
        origin = m_transform.pointLocalToWorld(origin);

        auto orientation = m_transform.orientation() * m_pose.orientation();

        m_arrows[0]->reset(origin, orientation * glm::vec3(1.0f, 0.0f, 0.0f));
        m_arrows[1]->reset(origin, orientation * glm::vec3(0.0f, 1.0f, 0.0f));
        m_arrows[2]->reset(origin, orientation * glm::vec3(0.0f, 0.0f, 1.0f));

        m_transformDirty = false;
    }

    for (auto &arrow : m_arrows) {
        arrow->render();
    }
}

DebugGeometryRenderer::DebugGeometryRenderer(DebugGeometryManager &manager) :
    m_manager(manager) {
    auto globalsLayout = manager.buildIns().shadedProgram.interface().uniformBlockRef("Globals").layout();
    m_globals = LayoutedBlob(globalsLayout, 1);
    m_globalsBuffer = m_manager.drawContext().createBuffer(globalsLayout);
}

bool DebugGeometryRenderer::visible() const {
    return m_visible;
}

DebugGeometryManager &DebugGeometryRenderer::manager() const {
    return m_manager;
}

const Buffer &DebugGeometryRenderer::globalsBuffer() const {
    return m_globalsBuffer;
}

void DebugGeometryRenderer::setVisible(bool visible) {
    m_visible = visible;
}

DebugBoxInstance &
DebugGeometryRenderer::addBox(const glm::vec3 &halfExtent, const glm::vec3 &color, bool wireframe, size_t index) {
    auto ptr = std::make_unique<DebugBoxInstance>(*this);

    if (index == NO_INDEX) index = m_boxes.emplace(std::move(ptr));
    else m_boxes.emplace_at(index, std::move(ptr));

    auto &box = *m_boxes[index];

    box.setHalfExtent(halfExtent);
    box.setColor(color);
    box.setWireframe(wireframe);
    box.setIndex(index);

    return box;
}

DebugPointInstance &DebugGeometryRenderer::addPoint(const glm::vec3 &position, const glm::vec3 &color, size_t index) {
    auto ptr = std::make_unique<DebugPointInstance>(*this);

    if (index == NO_INDEX) index = m_points.emplace(std::move(ptr));
    else m_points.emplace_at(index, std::move(ptr));

    auto &point = *m_points[index];

    point.setTransform(Transform3D::atPosition(position));
    point.setColor(color);
    point.setIndex(index);

    return point;
}

DebugArrowInstance &
DebugGeometryRenderer::addArrow(const glm::vec3 &origin, const glm::vec3 &delta, const glm::vec3 &color, size_t index) {
    auto ptr = std::make_unique<DebugArrowInstance>(*this);

    if (index == NO_INDEX) index = m_arrows.emplace(std::move(ptr));
    else m_arrows.emplace_at(index, std::move(ptr));

    auto &arrow = *m_arrows[index];

    arrow.reset(origin, delta);
    arrow.setColor(color);
    arrow.setIndex(index);

    return arrow;
}

DebugWireframeInstance &DebugGeometryRenderer::addWireframe(const glm::vec3 &color, size_t index) {
    auto ptr = std::make_unique<DebugWireframeInstance>(*this);

    if (index == NO_INDEX) index = m_wireframes.emplace(std::move(ptr));
    else m_wireframes.emplace_at(index, std::move(ptr));

    auto &wireframe = *m_wireframes[index];

    wireframe.setColor(color);
    wireframe.setIndex(index);

    return wireframe;
}

DebugSphereInstance &DebugGeometryRenderer::addSphere(const glm::vec3 &color, float radius, size_t index) {
    auto ptr = std::make_unique<DebugSphereInstance>(*this);

    if (index == NO_INDEX) index = m_spheres.emplace(std::move(ptr));
    else m_spheres.emplace_at(index, std::move(ptr));

    auto &sphere = *m_spheres[index];

    sphere.setRadius(radius);
    sphere.setColor(color);
    sphere.setIndex(index);

    return sphere;
}

DebugPoseInstance &DebugGeometryRenderer::addPose(const Pose3D &pose, size_t index) {
    auto ptr = std::make_unique<DebugPoseInstance>(*this);

    if (index == NO_INDEX) index = m_poses.emplace(std::move(ptr));
    else m_poses.emplace_at(index, std::move(ptr));

    auto &poseInstance = *m_poses[index];

    poseInstance.setPose(pose);
    poseInstance.setIndex(index);

    return poseInstance;
}

DebugBoxInstance &DebugGeometryRenderer::box(size_t index) {
    if (m_boxes.contains(index)) return *m_boxes[index];
    return addBox({}, {}, false, index);
}

DebugPointInstance &DebugGeometryRenderer::point(size_t index) {
    if (m_points.contains(index)) return *m_points[index];
    return addPoint({}, {}, index);
}

DebugArrowInstance &DebugGeometryRenderer::arrow(size_t index) {
    if (m_arrows.contains(index)) return *m_arrows[index];
    return addArrow({}, {}, {}, index);
}

DebugWireframeInstance &DebugGeometryRenderer::wireframe(size_t index) {
    if (m_wireframes.contains(index)) return *m_wireframes[index];
    return addWireframe({}, index);
}

DebugSphereInstance &DebugGeometryRenderer::sphere(size_t index) {
    if (m_spheres.contains(index)) return *m_spheres[index];
    return addSphere({}, {}, index);
}

DebugPoseInstance &DebugGeometryRenderer::pose(size_t index) {
    if (m_poses.contains(index)) return *m_poses[index];
    return addPose({}, index);
}

void DebugGeometryRenderer::removeBox(size_t index) {
    Assert(m_boxes.contains(index), "Invalid index");
    m_boxes.erase(index);
}

void DebugGeometryRenderer::removePoint(size_t index) {
    Assert(m_points.contains(index), "Invalid index");
    m_points.erase(index);
}

void DebugGeometryRenderer::removeArrow(size_t index) {
    Assert(m_arrows.contains(index), "Invalid index");
    m_arrows.erase(index);
}

void DebugGeometryRenderer::removeWireframe(size_t index) {
    Assert(m_wireframes.contains(index), "Invalid index");
    m_wireframes.erase(index);
}

void DebugGeometryRenderer::removeSphere(size_t index) {
    Assert(m_spheres.contains(index), "Invalid index");
    m_spheres.erase(index);
}

void DebugGeometryRenderer::removePose(size_t index) {
    Assert(m_poses.contains(index), "Invalid index");
    m_poses.erase(index);
}

void DebugGeometryRenderer::render(const Camera3D &camera) {
    m_globals.field<glm::mat4>("ViewProjection")[0] = camera.viewProjection();
    m_globalsBuffer.upload(m_globals);

    for (auto &box : m_boxes) box->render();
    for (auto &point : m_points) point->render();
    for (auto &arrow : m_arrows) arrow->render();
    for (auto &wireframe : m_wireframes) wireframe->render();
    for (auto &sphere : m_spheres) sphere->render();
    for (auto &pose : m_poses) pose->render();
}

}
