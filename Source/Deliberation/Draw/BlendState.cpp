#include <Deliberation/Draw/BlendState.h>

#include <sstream>

#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

BlendState BlendState::disabled()
{
    return BlendState();
}

BlendState::BlendState():
    m_enabled(false),
    m_equation(BlendEquation::None),
    m_sfactor(BlendFactor::None),
    m_dfactor(BlendFactor::None)
{

}

BlendState::BlendState(BlendEquation equation, BlendFactor sfactor, BlendFactor dfactor):
    m_enabled(true),
    m_equation(equation),
    m_sfactor(sfactor),
    m_dfactor(dfactor)
{
    Assert(m_equation != BlendEquation::None, "");
    Assert(m_sfactor != BlendFactor::None, "");
    Assert(m_dfactor != BlendFactor::None, "");
}

bool BlendState::enabled() const
{
    return m_enabled;
}

BlendEquation BlendState::equation() const
{
    return m_equation;
}

BlendFactor BlendState::sfactor() const
{
    return m_sfactor;
}

BlendFactor BlendState::dfactor() const
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
    stream << "Equation: " << (gl::GLenum)m_equation << "; SrcFactor: " << (gl::GLenum)m_sfactor << "; DstFactor: " << (gl::GLenum)m_dfactor;

    return stream.str();
}

}



