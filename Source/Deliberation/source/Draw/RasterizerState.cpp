#include <Deliberation/Draw/RasterizerState.h>

#include <glbinding/Meta.h>
#include <glbinding/gl/functions.h>

namespace deliberation
{

RasterizerState::RasterizerState():
    m_primitive(gl::GL_TRIANGLES),
    m_pointSize(1.0f),
    m_lineWidth(1.0f)
{

}

RasterizerState::RasterizerState(gl::GLenum primitive):
    m_primitive(primitive)
{

}

RasterizerState::RasterizerState(gl::GLenum primitive,
                                 gl::GLfloat pointSize,
                                 gl::GLfloat lineWidth):
    m_primitive(primitive),
    m_pointSize(pointSize),
    m_lineWidth(lineWidth)
{

}

gl::GLenum RasterizerState::primitive() const
{
    return m_primitive;
}

gl::GLfloat RasterizerState::pointSize() const
{
    return m_pointSize;
}

gl::GLfloat RasterizerState::lineWidth() const
{
    return m_lineWidth;
}

void RasterizerState::setPrimitive(gl::GLenum primitive)
{
    m_primitive = primitive;
}

void RasterizerState::setPointSize(gl::GLfloat pointSize)
{
    m_pointSize = pointSize;
}

void RasterizerState::setLineWidth(gl::GLfloat lineWidth)
{
    m_lineWidth = lineWidth;
}

std::string RasterizerState::toString() const
{
    return "PointSize: " + std::to_string(m_pointSize) + "; LineWidth: " + std::to_string(m_lineWidth) + "; PrimitiveType: " + glbinding::Meta::getString(m_primitive);
}

}

