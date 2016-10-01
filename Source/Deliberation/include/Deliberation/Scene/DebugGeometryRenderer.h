#pragma once

#include <stdlib.h>
#include <vector>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Context;
class Camera3D;
class DebugGeometryRenderer;
class Transform3D;

class DELIBERATION_API DebugGeometryInstance
{
public:
    bool visible() const;
    void setVisible(bool visible);

protected:
    DebugGeometryInstance();

private:
    bool m_visible = true;
};

class DELIBERATION_API DebugBoxInstance final:
    public DebugGeometryInstance
{
public:
    DebugBoxInstance(const Draw & draw,
                     const glm::vec3 & halfExtent,
                     const glm::vec3 & color);

    Transform3D & transform();

    Box toBox() const;

private:
    friend class DebugGeometryRenderer;

private:
    Draw        m_draw;
    Transform3D m_transform;
    glm::vec3   m_halfExtent;
    glm::vec3   m_color;
};

class DELIBERATION_API DebugPointInstance final:
    public DebugGeometryInstance
{
public:
    DebugPointInstance(Program & program,
                       const DataLayout & vertexLayout,
                       const glm::vec3 & position,
                       const glm::vec3 & color);

    void setPosition(const glm::vec3 & position);

private:
    friend class DebugGeometryRenderer;

private:
    void build(const Program & program,
               const DataLayout & vertexLayout,
               const glm::vec3 & color);

private:
    Draw        m_draw;
    glm::vec3   m_position;
    Buffer      m_vertexBuffer;
    glm::vec3   m_color;
};

class DELIBERATION_API DebugArrowInstance final:
    public DebugGeometryInstance
{
public:
    DebugArrowInstance(const Program & program,
                       const DataLayout & vertexLayout,
                       const glm::vec3 & origin,
                       const glm::vec3 & delta,
                       const glm::vec3 & color);

    void reset(const glm::vec3 & origin, const glm::vec3 & delta);

private:
    friend class DebugGeometryRenderer;

private:
    void buildVertices();

private:
    Draw      m_draw;
    Buffer    m_vertexBuffer;
    glm::vec3 m_origin;
    glm::vec3 m_delta;
    glm::vec3 m_color;
};

class DELIBERATION_API DebugGeometryRenderer final
{
public:
    DebugGeometryRenderer(Context & context, const Camera3D & camera);

    bool visible() const;

    void setVisible(bool visible);

    DebugBoxInstance & box(size_t index);
    DebugArrowInstance & arrow(size_t index);
    DebugPointInstance & point(size_t index);

//    void allocateBoxes(uint count, bool wireframe);
    void allocatePoints(uint count, const glm::vec3 & color = {1.0f, 0.0f, .5f}, bool visible = false);
    void allocateArrows(uint count, const glm::vec3 & color = {1.0f, 0.0f, .5f}, bool visible = false);

    size_t addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe = false);
    size_t addPoint(const glm::vec3 & position, const glm::vec3 & color);
    size_t addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color);

    void removeBox(size_t index);
    void removePoint(size_t index);
    void removeArrow(size_t index);

    void schedule();

private:
    friend class DebugBoxInstance;

private:
    Context &                       m_context;
    const Camera3D &                m_camera;

    bool                            m_visible = true;

    Program                         m_shadedProgram;
    Program                         m_unicolorProgram;

    DataLayout                      m_unicolorDataLayout;

    Buffer                          m_boxTrianglesVertexBuffer;
    Buffer                          m_boxTrianglesIndexBuffer;
    Buffer                          m_boxLinesVertexBuffer;
    Buffer                          m_boxLinesIndexBuffer;

    std::vector<DebugBoxInstance>   m_boxes;
    std::vector<DebugArrowInstance> m_arrows;
    std::vector<DebugPointInstance> m_points;
};

}