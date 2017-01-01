#pragma once

#include <stdlib.h>
#include <vector>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/Optional.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Scene/Vertices.h>
#include <Deliberation/Scene/Debug/DebugGeometryManager.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Context;
class Camera3D;
class DebugGeometryRenderer;
class DebugGeometryManager;
class Transform3D;

class DELIBERATION_API DebugGeometryInstance
{
public:
    DebugGeometryInstance(DebugGeometryRenderer & renderer,
                          size_t index);

    size_t index() const;
    DebugGeometryRenderer & renderer() const;
    DebugGeometryManager & manager() const;
    const DebugGeometryManager::BuildIns & buildIns() const;
    bool visible() const;
    const Transform3D & transform() const;

    void setVisible(bool visible);
    void setTransform(const Transform3D & transform);

protected:
    size_t                  m_index = 0;
    DebugGeometryRenderer & m_renderer;
    bool                    m_visible = true;
    Transform3D             m_transform;
    mutable bool            m_transformDirty = true;
};

class DELIBERATION_API DebugBoxInstance final:
    public DebugGeometryInstance
{
public:
    DebugBoxInstance(DebugGeometryRenderer & renderer,
                     size_t index);

    bool wireframe() const;
    const glm::vec3 & halfExtent() const;
    const glm::vec3 & color() const;

    void setHalfExtent(const glm::vec3 & halfExtent);
    void setColor(const glm::vec3 & color);
    void setWireframe(bool wireframe);

    void schedule() const;

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

class DELIBERATION_API DebugPointInstance final:
    public DebugGeometryInstance
{
public:
    DebugPointInstance(DebugGeometryRenderer & renderer,
                       size_t index);

    const glm::vec3 & color() const;

    void setColor(const glm::vec3 & color);

    void schedule() const;

private:
    Draw        m_draw;
    glm::vec3   m_color;
    Uniform     m_colorUniform;
};

class DELIBERATION_API DebugArrowInstance final:
    public DebugGeometryInstance
{
public:
    DebugArrowInstance(DebugGeometryRenderer & renderer,
                       size_t index);

    const glm::vec3 & origin() const;
    const glm::vec3 & delta() const;
    const glm::vec3 & color() const;

    void setOrigin(const glm::vec3 & origin);
    void setDelta(const glm::vec3 & delta);
    void setColor(const glm::vec3 & color);

    void reset(const glm::vec3 & origin, const glm::vec3 & delta);

    void schedule() const;

private:
    Draw            m_lineDraw;
    Buffer          m_lineVertexBuffer;
    LayoutedBlob    m_lineVertices;
    Uniform         m_lineColorUniform;

    Draw            m_coneDraw;
    Uniform         m_coneColorUniform;
    Uniform         m_coneTransformUniform;

    glm::vec3       m_origin;
    glm::vec3       m_delta;
    glm::vec3       m_color;
};

class DELIBERATION_API DebugWireframeInstance final:
    public DebugGeometryInstance
{
public:
    DebugWireframeInstance(DebugGeometryRenderer & renderer,
                           size_t index);

    void addLineStrip(const std::vector<ColoredVertex> & vertices);

    void schedule() const;

private:
    Optional<Draw>                          m_draw;
    std::vector<std::vector<ColoredVertex>> m_lineStrips;
    uint                                    m_lineCount = 0;
};

class DELIBERATION_API DebugSphereInstance final:
    public DebugGeometryInstance
{
public:
    DebugSphereInstance(DebugGeometryRenderer & renderer,
                        size_t index);

    const glm::vec3 & color() const;
    float radius() const;

    void setColor(const glm::vec3 & color);
    void setRadius(float radius);

    void schedule() const;

private:
    Draw            m_draw;
    Uniform         m_colorUniform;
    mutable Uniform m_transformUniform;
    glm::vec3       m_color;
    float           m_radius = 0.0f;
    bool            m_dirty = true;
};

class DELIBERATION_API DebugPoseInstance final:
    public DebugGeometryInstance
{
public:
    DebugPoseInstance(DebugGeometryRenderer & renderer,
                      size_t index);

    const Pose3D & pose() const;

    void setPose(const Pose3D & pose);

    void schedule(const Camera3D & camera) const;

private:
    struct Arrow {
        Draw   lineDraw;
        Buffer lineVertexBuffer;
        Draw   coneDraw;
    };

private:
    std::array<Arrow, 3> m_arrows;
};

class DELIBERATION_API DebugGeometryRenderer final
{
public:
    DebugGeometryRenderer(DebugGeometryManager & manager);

    bool visible() const;
    DebugGeometryManager & manager() const;
    const Buffer & globalsBuffer() const;

    void setVisible(bool visible);

    DebugBoxInstance & box(size_t index);
    DebugArrowInstance & arrow(size_t index);
    DebugPointInstance & point(size_t index);
    DebugWireframeInstance & wireframe(size_t index);
    DebugSphereInstance & sphere(size_t index);
    DebugPoseInstance & pose(size_t index);

    size_t numBoxes() const;
    size_t numPoints() const;
    size_t numArrows() const;
    size_t numWireframes() const;
    size_t numSpheres() const;
    size_t numPoses() const;

    void resizeBoxes(size_t count);
    void resizePoints(size_t count);
    void resizeArrows(size_t count);
    void resizeWireframes(size_t count);
    void resizeSpheres(size_t count);
    void resizePoses(size_t count);

    DebugBoxInstance & addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe = false);
    DebugPointInstance & addPoint(const glm::vec3 & position, const glm::vec3 & color);
    DebugArrowInstance & addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color);
    DebugWireframeInstance & addWireframe();
    DebugSphereInstance & addSphere(const glm::vec3 & color, float radius);
    DebugPoseInstance & addPose(const Pose3D & pose);

    void removeBox(size_t index);
    void removePoint(size_t index);
    void removeArrow(size_t index);
    void removeWireframe(size_t index);
    void removeSphere(size_t index);
    void removePose(size_t index);

    void schedule(const Camera3D & camera);

private:
    DebugGeometryManager &                               m_manager;

    Buffer                                               m_globalsBuffer;
    LayoutedBlob                                         m_globals;

    bool                                                 m_visible = true;

    std::vector<std::unique_ptr<DebugBoxInstance>>       m_boxes;
    std::vector<std::unique_ptr<DebugArrowInstance>>     m_arrows;
    std::vector<std::unique_ptr<DebugPointInstance>>     m_points;
    std::vector<std::unique_ptr<DebugWireframeInstance>> m_wireframes;
    std::vector<std::unique_ptr<DebugSphereInstance>>    m_spheres;
    std::vector<std::unique_ptr<DebugPoseInstance>>      m_poses;
};

}