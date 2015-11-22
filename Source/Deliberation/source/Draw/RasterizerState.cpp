#include <Deliberation/Draw/RasterizerState.h>

#include <glbinding/gl/functions.h>

namespace deliberation
{

RasterizerState::RasterizerState():
    m_pointSize(1.0f),
    m_lineWidth(1.0f)
{

}

RasterizerState::RasterizerState(gl::GLfloat pointSize,
                                 gl::GLfloat lineWidth):
    m_pointSize(pointSize),
    m_lineWidth(lineWidth)
{

}

gl::GLfloat RasterizerState::pointSize() const
{
    return m_pointSize;
}

gl::GLfloat RasterizerState::lineWidth() const
{
    return m_lineWidth;
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
    return "PointSize: " + std::to_string(m_pointSize) + "; LineWidth: " + std::to_string(m_lineWidth);
}

}

