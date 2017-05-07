#pragma once

#include <stdlib.h>
#include <vector>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Optional.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Vertices.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
class DrawContext;
class Camera3D;
class DebugGeometryRenderer;
class DebugGeometryManager;
class Transform3D;

class DebugGeometryInstance
{
  public:
    DebugGeometryInstance(DebugGeometryRenderer & renderer);

    size_t                                 index() const;
    DebugGeometryRenderer &                renderer() const;
    DebugGeometryManager &                 manager() const;
    const DebugGeometryManager::BuildIns & buildIns() const;
    bool                                   visible() const;
    const Transform3D &                    transform() const;

    void setIndex(size_t index);
    void setVisible(bool visible);
    void setTransform(const Transform3D & transform);

  protected:
    size_t                  m_index = 0;
    DebugGeometryRenderer & m_renderer;
    bool                    m_visible = true;
    Transform3D             m_transform;
    mutable bool            m_transformDirty = true;
};

class DebugBoxInstance final : public DebugGeometryInstance
{
  public:
    DebugBoxInstance(DebugGeometryRenderer & renderer);

    bool              wireframe() const;
    const glm::vec3 & halfExtent() const;
    const glm::vec3 & color() const;

    void setHalfExtent(const glm::vec3 & halfExtent);
    void setColor(const glm::vec3 & color);
    void setWireframe(bool wireframe);

    void render() const;

    Box toBox() const;

  private:
    mutable Draw    m_draw;
    glm::vec3       m_halfExtent;
    glm::vec3       m_color;
    bool            m_wireframe = false;
    mutable bool    m_meshDirty = true;
    mutable bool    m_colorDirty = true;
    mutable Uniform m_transformUniform;
};

class DebugPointInstance final : public DebugGeometryInstance
{
  public:
    DebugPointInstance(DebugGeometryRenderer & renderer);

    const glm::vec3 & color() const;

    void setColor(const glm::vec3 & color);

    void render() const;

  private:
    Draw            m_draw;
    mutable Uniform m_transformUniform;
    glm::vec3       m_color;
    Uniform         m_colorUniform;
};

class DebugArrowInstance final : public DebugGeometryInstance
{
  public:
    DebugArrowInstance(DebugGeometryRenderer & renderer);

    const glm::vec3 & origin() const;
    const glm::vec3 & delta() const;
    const glm::vec3 & color() const;

    void setOrigin(const glm::vec3 & origin);
    void setDelta(const glm::vec3 & delta);
    void setColor(const glm::vec3 & color);

    void reset(const glm::vec3 & origin, const glm::vec3 & delta);

    void render() const;

  private:
    Draw         m_lineDraw;
    Buffer       m_lineVertexBuffer;
    LayoutedBlob m_lineVertices;
    Uniform      m_lineColorUniform;

    Draw    m_coneDraw;
    Uniform m_coneColorUniform;
    Uniform m_coneTransformUniform;

    glm::vec3 m_origin;
    glm::vec3 m_delta;
    glm::vec3 m_color;
};

class DebugWireframeInstance final : public DebugGeometryInstance
{
  public:
    DebugWireframeInstance(DebugGeometryRenderer & renderer);

    const glm::vec3 & color() const;

    void setColor(const glm::vec3 & color);

    void addLineStrip(const std::vector<BasicVertex> & vertices);

    void render() const;

  private:
    Draw            m_draw;
    Buffer          m_vertexBuffer;
    mutable Uniform m_transformUniform;
    Uniform         m_colorUniform;
    LayoutedBlob    m_vertices;
    glm::vec3       m_color;
};

class DebugSphereInstance final : public DebugGeometryInstance
{
  public:
    DebugSphereInstance(DebugGeometryRenderer & renderer);

    const glm::vec3 & color() const;
    float             radius() const;

    void setColor(const glm::vec3 & color);
    void setRadius(float radius);

    void render() const;

  private:
    Draw            m_draw;
    Uniform         m_colorUniform;
    mutable Uniform m_transformUniform;
    glm::vec3       m_color;
    float           m_radius = 0.0f;
    bool            m_dirty = true;
};

class DebugPoseInstance final : public DebugGeometryInstance
{
  public:
    DebugPoseInstance(DebugGeometryRenderer & renderer);

    const Pose3D & pose() const;

    void setPose(const Pose3D & pose);

    void render() const;

  private:
    Draw                                                       m_draw;
    Pose3D                                                     m_pose;
    mutable std::array<boost::optional<DebugArrowInstance>, 3> m_arrows;
};

class DebugGeometryRenderer final
{
  public:
    static constexpr size_t NO_INDEX = std::numeric_limits<size_t>::max();

  public:
    DebugGeometryRenderer(DebugGeometryManager & manager);

    bool                   visible() const;
    DebugGeometryManager & manager() const;
    const Buffer &         globalsBuffer() const;

    void setVisible(bool visible);

    DebugBoxInstance & addBox(
        const glm::vec3 & halfExtent,
        const glm::vec3 & color,
        bool              wireframe = false,
        size_t            index = NO_INDEX);
    DebugPointInstance & addPoint(
        const glm::vec3 & position,
        const glm::vec3 & color,
        size_t            index = NO_INDEX);
    DebugArrowInstance & addArrow(
        const glm::vec3 & origin,
        const glm::vec3 & delta,
        const glm::vec3 & color,
        size_t            index = NO_INDEX);
    DebugWireframeInstance &
    addWireframe(const glm::vec3 & color, size_t index = NO_INDEX);
    DebugSphereInstance &
                        addSphere(const glm::vec3 & color, float radius, size_t index = NO_INDEX);
    DebugPoseInstance & addPose(const Pose3D & pose, size_t index = NO_INDEX);

    DebugBoxInstance &       box(size_t index);
    DebugPointInstance &     point(size_t index);
    DebugArrowInstance &     arrow(size_t index);
    DebugWireframeInstance & wireframe(size_t index);
    DebugSphereInstance &    sphere(size_t index);
    DebugPoseInstance &      pose(size_t index);

    void removeBox(size_t index);
    void removePoint(size_t index);
    void removeArrow(size_t index);
    void removeWireframe(size_t index);
    void removeSphere(size_t index);
    void removePose(size_t index);

    void render(const Camera3D & camera);

  private:
    DebugGeometryManager & m_manager;

    Buffer       m_globalsBuffer;
    LayoutedBlob m_globals;

    bool m_visible = true;

    SparseVector<std::unique_ptr<DebugBoxInstance>>       m_boxes;
    SparseVector<std::unique_ptr<DebugArrowInstance>>     m_arrows;
    SparseVector<std::unique_ptr<DebugPointInstance>>     m_points;
    SparseVector<std::unique_ptr<DebugWireframeInstance>> m_wireframes;
    SparseVector<std::unique_ptr<DebugSphereInstance>>    m_spheres;
    SparseVector<std::unique_ptr<DebugPoseInstance>>      m_poses;
};
}