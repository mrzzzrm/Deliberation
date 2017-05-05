#pragma once

#include <string>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

enum class DrawPrimitive: unsigned int
{
    Points                  = (unsigned int)gl::GL_POINTS,
    LineStrip               = (unsigned int)gl::GL_LINE_STRIP,
    LineLoop                = (unsigned int)gl::GL_LINE_LOOP,
    Lines                   = (unsigned int)gl::GL_LINES,
    LineStripAdjacency      = (unsigned int)gl::GL_LINE_STRIP_ADJACENCY,
    LinesAdjacency          = (unsigned int)gl::GL_LINES_ADJACENCY,
    TriangleStrip           = (unsigned int)gl::GL_TRIANGLE_STRIP,
    TriangleFan             = (unsigned int)gl::GL_TRIANGLE_FAN,
    Triangles               = (unsigned int)gl::GL_TRIANGLES,
    TriangleStripAdjacency  = (unsigned int)gl::GL_TRIANGLE_STRIP_ADJACENCY,
    TrianglesAdjacency      = (unsigned int)gl::GL_TRIANGLES_ADJACENCY,
    Patches                 = (unsigned int)gl::GL_PATCHES
};

class RasterizerState final
{
public:
    RasterizerState();
    RasterizerState(DrawPrimitive primitive);
    RasterizerState(DrawPrimitive primitive,
                    float pointSize,
                    float lineWidth);

    DrawPrimitive primitive() const;
    float pointSize() const;
    float lineWidth() const;
    const glm::uvec4 & scissorRect() const;
    bool scissorRectEnabled() const;

    void setPrimitive(DrawPrimitive primitive);
    void setPointSize(float pointSize);
    void setLineWidth(float lineWidth);

    void enableScissorRect(u32 x, u32 y, u32 width, u32 height);
    void disableScissorRect();

    std::string toString() const;

private:
    DrawPrimitive   m_primitive;
    float           m_pointSize;
    float           m_lineWidth;
    glm::uvec4      m_scissorRect;
    bool            m_scissorRectEnabled = false;
};

}

