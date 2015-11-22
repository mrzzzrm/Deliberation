#include <Deliberation/Draw/DepthState.h>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{

DepthState::DepthState():
    m_depthTestEnabled(true),
    m_depthMask(true)
{
}

DepthState::DepthState(bool depthTestEnabled, bool depthMask):
    m_depthTestEnabled(depthTestEnabled),
    m_depthMask(depthMask)
{
}

bool DepthState::depthTest() const
{
    return m_depthTestEnabled;
}

bool DepthState::depthMask() const
{
    return m_depthMask;
}

void DepthState::setDepthTest(bool enabled)
{
    m_depthTestEnabled = enabled;
}

void DepthState::setDepthMask(bool mask)
{
    m_depthMask = mask;
}

std::string DepthState::toString() const
{
    return std::string("DepthTestEnabled: ") + (m_depthTestEnabled ? "true" : "false") + "; DepthMask: " + (m_depthMask ? "true" : "false");
}

}

