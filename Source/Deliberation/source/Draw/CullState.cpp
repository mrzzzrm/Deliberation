#include <Deliberation/Draw/CullState.h>

#include <sstream>

#include <glbinding/gl/functions.h>

namespace deliberation
{

CullState CullState::disabled()
{
    CullState cullState;
    cullState.setEnabled(false);

    return cullState;
}

CullState::CullState(gl::GLenum cullFace, gl::GLenum frontFace):
    m_enabled(true),
    m_cullFace(cullFace),
    m_frontFace(frontFace)
{

}

bool CullState::enabled() const
{
    return m_enabled;
}

gl::GLenum CullState::cullFace() const
{
    return m_cullFace;
}

gl::GLenum CullState::frontFace() const
{
    return m_frontFace;
}

void CullState::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void CullState::setCullFace(gl::GLenum cullFace)
{
    m_cullFace = cullFace;
}

void CullState::setFrontFace(gl::GLenum frontFace)
{
    m_frontFace = frontFace;
}

std::string CullState::toString() const
{
    if (!m_enabled)
    {
        return "Disabled";
    }

    std::stringstream stream;
    stream << "CullFace: " << m_cullFace;
    stream << "FrontFace: " << m_frontFace;

    return stream.str();
}

}

