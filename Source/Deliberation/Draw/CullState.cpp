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

CullState::CullState(CullFace cullFace, FrontFace frontFace):
    m_enabled(true),
    m_cullFace(cullFace),
    m_frontFace(frontFace)
{

}

bool CullState::enabled() const
{
    return m_enabled;
}

CullFace CullState::cullFace() const
{
    return m_cullFace;
}

FrontFace CullState::frontFace() const
{
    return m_frontFace;
}

void CullState::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void CullState::setCullFace(CullFace cullFace)
{
    m_cullFace = cullFace;
}

void CullState::setFrontFace(FrontFace frontFace)
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
    stream << "CullFace: " << (gl::GLenum)m_cullFace;
    stream << "FrontFace: " << (gl::GLenum)m_frontFace;

    return stream.str();
}

}

