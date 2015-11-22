#include <Deliberation/Draw/BlendState.h>

#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

namespace deliberation
{

BlendState BlendState::disabled()
{
    return BlendState();
}

BlendState::BlendState():
    m_enabled(false),
    m_equation(gl::GL_NONE),
    m_sfactor(gl::GL_NONE),
    m_dfactor(gl::GL_NONE)
{

}

BlendState::BlendState(gl::GLenum equation, gl::GLenum sfactor, gl::GLenum dfactor):
    m_enabled(true),
    m_equation(equation),
    m_sfactor(sfactor),
    m_dfactor(dfactor)
{

}

bool BlendState::enabled() const
{
    return m_enabled;
}

gl::GLenum BlendState::equation() const
{
    return m_equation;
}

gl::GLenum BlendState::sfactor() const
{
    return m_sfactor;
}

gl::GLenum BlendState::dfactor() const
{
    return m_dfactor;
}

std::string BlendState::toString() const
{
    if (!m_enabled)
    {
        return "Disabled";
    }

    std::stringstream stream;
    stream << "Equation: " << m_equation << "; SrcFactor: " << m_sfactor << "; DstFactor: " << m_dfactor;

    return stream.str();
}

}



