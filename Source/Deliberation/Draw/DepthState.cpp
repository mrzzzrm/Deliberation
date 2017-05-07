#include <Deliberation/Draw/DepthState.h>

#include <glbinding/gl/boolean.h>
#include <glbinding/gl/enum.h>
#include <glbinding/gl/functions.h>

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
DepthState DepthState::enabled()
{
    return {DepthTest::Enabled, DepthWrite::Enabled};
}

DepthState DepthState::disabledRW()
{
    return {DepthTest::Disabled, DepthWrite::Disabled};
}

DepthState DepthState::disabledR()
{
    return {DepthTest::Always, DepthWrite::Enabled};
}

DepthState DepthState::disabledW(DepthTest depthTest)
{
    return {depthTest, DepthWrite::Disabled};
}

DepthState::DepthState(DepthTest depthTest, DepthWrite depthWrite)
    : m_depthTest(depthTest), m_depthWrite(depthWrite)
{
}

DepthTest DepthState::depthTest() const { return m_depthTest; }

DepthWrite DepthState::depthWrite() const { return m_depthWrite; }

void DepthState::setDepthTest(DepthTest depthTest) { m_depthTest = depthTest; }

void DepthState::setDepthWrite(DepthWrite depthWrite)
{
    m_depthWrite = depthWrite;
}

std::string DepthState::toString() const { return {}; }
}
