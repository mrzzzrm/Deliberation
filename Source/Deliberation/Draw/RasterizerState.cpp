#include <Deliberation/Draw/RasterizerState.h>

#include <glbinding/Meta.h>
#include <glbinding/gl/functions.h>

namespace deliberation
{

RasterizerState::RasterizerState():
    m_primitive(DrawPrimitive::Triangles),
    m_pointSize(1.0f),
    m_lineWidth(1.0f)
{

}

RasterizerState::RasterizerState(DrawPrimitive primitive):
    m_primitive(primitive)
{

}

RasterizerState::RasterizerState(DrawPrimitive primitive,
                                 float pointSize,
                                 float lineWidth):
    m_primitive(primitive),
    m_pointSize(pointSize),
    m_lineWidth(lineWidth)
{

}

DrawPrimitive RasterizerState::primitive() const
{
    return m_primitive;
}

float RasterizerState::pointSize() const
{
    return m_pointSize;
}

float RasterizerState::lineWidth() const
{
    return m_lineWidth;
}

const glm::uvec4 & RasterizerState::scissorRect() const
{
    return m_scissorRect;
}

bool RasterizerState::scissorRectEnabled() const
{
    return m_scissorRectEnabled;
}

void RasterizerState::setPrimitive(DrawPrimitive primitive)
{
    m_primitive = primitive;
}

void RasterizerState::setPointSize(float pointSize)
{
    m_pointSize = pointSize;
}

void RasterizerState::setLineWidth(float lineWidth)
{
    m_lineWidth = lineWidth;
}

void RasterizerState::enableScissorRect(u32 x, u32 y, u32 width, u32 height)
{
    m_scissorRect = {x, y, width, height};
    m_scissorRectEnabled = true;
}

void RasterizerState::disableScissorRect()
{
    m_scissorRectEnabled = false;
}

std::string RasterizerState::toString() const
{
    return "PointSize: " + std::to_string(m_pointSize) + "; LineWidth: " + std::to_string(m_lineWidth) +
        "; PrimitiveType: " + glbinding::Meta::getString((gl::GLenum)m_primitive);
}

}

