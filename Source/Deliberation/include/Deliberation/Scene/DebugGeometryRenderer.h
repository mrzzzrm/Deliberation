#pragma once

#include <stdlib.h>
#include <vector>

#include <Deliberation/Core/Math/Box.h>
#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Draw/Buffer.h>
#include <Deliberation/Draw/Draw.h>
#include <Deliberation/Draw/Uniform.h>

#include <Deliberation/Scene/Vertices.h>

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

    void setTransform(const Transform3D & transform);

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
    DebugArrowInstance(const Program & lineProgram,
                       const DataLayout & lineVertexLayout,
                       const Draw & coneDraw,
                       const glm::vec3 & origin,
                       const glm::vec3 & delta,
                       const glm::vec3 & color);

    void reset(const glm::vec3 & origin, const glm::vec3 & delta);

    void setColor(const glm::vec3 & color);

private:
    friend class DebugGeometryRenderer;

private:
    void buildVertices();
    void setupConeTransform();

private:
    Draw      m_lineDraw;
    Buffer    m_lineVertexBuffer;

    Draw      m_coneDraw;

    glm::vec3 m_origin;
    glm::vec3 m_delta;
    glm::vec3 m_color;
};

class DELIBERATION_API DebugWireframeInstance final:
    public DebugGeometryInstance
{
public:
    DebugWireframeInstance(const Program & program,
                           const DataLayout & vertexLayout);

    void setTransform(const Transform3D & transform);
    void addLineStrip(const std::vector<ColoredVertex> & vertices);

    void schedule(const Camera3D & camera);

private:
    friend class DebugGeometryRenderer;

private:
    Draw                                    m_draw;
    Program                                 m_program;
    DataLayout                              m_vertexLayout;
    Transform3D                             m_transform;
    bool                                    m_dirty = true;
    std::vector<std::vector<ColoredVertex>> m_lineStrips;
    u32                                     m_lineCount = 0;
};

class DELIBERATION_API DebugSphereInstance final:
    public DebugGeometryInstance
{
public:
    DebugSphereInstance(const Program & program,
                        const Buffer & vertexBuffer,
                        const Buffer & indexBuffer,
                        const glm::vec3 & color,
                        const float scale);

    void setTransform(const Transform3D & transform);

    void schedule(const Camera3D & camera);

private:
    Draw        m_draw;
    Transform3D m_transform;
    Program     m_program;
    Buffer      m_vertexBuffer;
    Buffer      m_indexBuffer;
    glm::vec3   m_color;
    float       m_scale;
    bool        m_dirty = true;
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
    DebugWireframeInstance & wireframe(size_t index);
    DebugWireframeInstance & sphere(size_t index);

    void resizeArrows(uint count, const glm::vec3 & color = {1.0f, 0.0f, .5f}, bool visible = false);

//    void allocateBoxes(uint count, bool wireframe);
    void allocatePoints(uint count, const glm::vec3 & color = {1.0f, 0.0f, .5f}, bool visible = false);
    void allocateArrows(uint count, const glm::vec3 & color = {1.0f, 0.0f, .5f}, bool visible = false);

    size_t addBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe = false);
    size_t addPoint(const glm::vec3 & position, const glm::vec3 & color);
    size_t addArrow(const glm::vec3 & origin, const glm::vec3 & delta, const glm::vec3 & color);
    size_t addWireframe();
    size_t addSphere(const glm::vec3 & color, float radius);

    DebugBoxInstance & addAndGetBox(const glm::vec3 & halfExtent, const glm::vec3 & color, bool wireframe = false);
    DebugWireframeInstance & addAndGetWireframe();
    DebugSphereInstance & addAndGetSphere(const glm::vec3 & color, float radius);

    void removeBox(size_t index);
    void removePoint(size_t index);
    void removeArrow(size_t index);

    void schedule();

private:
    friend class DebugBoxInstance;

private:
    Context &                           m_context;
    const Camera3D &                    m_camera;

    bool                                m_visible = true;

    Program                             m_shadedProgram;
    Program                             m_unicolorProgram;
    Program                             m_vertexColorProgram;

    DataLayout                          m_unicolorDataLayout;
    DataLayout                          m_vertexColorDataLayout;

    Buffer                              m_boxTrianglesVertexBuffer;
    Buffer                              m_boxTrianglesIndexBuffer;
    Buffer                              m_boxLinesVertexBuffer;
    Buffer                              m_boxLinesIndexBuffer;

    Buffer                              m_coneVertexBuffer;
    Buffer                              m_coneIndexBuffer;

    Buffer                              m_sphereVertexBuffer;
    Buffer                              m_sphereIndexBuffer;

    /**
     * TODO: Make unique_ptr
     */
    std::vector<DebugBoxInstance>       m_boxes;
    std::vector<DebugArrowInstance>     m_arrows;
    std::vector<DebugPointInstance>     m_points;
    std::vector<DebugWireframeInstance> m_wireframes;
    std::vector<DebugSphereInstance>    m_spheres;
};

}