#include <Deliberation/Draw/CullState.h>

#include <sstream>

#include <glbinding/gl/functions.h>

namespace deliberation
{

CullState CullState::disabled()
{
    return CullState(false, gl::GL_BACK);
}

CullState::CullState():
    m_enabled(true),
    m_mode(gl::GL_BACK)
{

}

CullState::CullState(bool enabled, gl::GLenum mode):
    m_enabled(enabled),
    m_mode(mode)
{

}

bool CullState::enabled() const
{
    return m_enabled;
}

gl::GLenum CullState::mode() const
{
    return m_mode;
}

void CullState::setEnabled(bool enabled)
{
    m_enabled = enabled;
}

void CullState::setMode(gl::GLenum mode)
{
    m_mode = mode;
}

std::string CullState::toString() const
{
    if (!m_enabled)
    {
        return "Disabled";
    }

    std::stringstream stream;
    stream << "Mode: " << m_mode;

    return stream.str();
}

}

