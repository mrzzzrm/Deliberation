#pragma once

#include <glbinding/gl/enum.h>

#include <string>

namespace deliberation
{

enum class DepthTest: unsigned int
{
    Less            = (unsigned int)gl::GL_LESS,
    Enabled         = (unsigned int)Less,
    Always          = (unsigned int)gl::GL_ALWAYS,
    Disabled        = (unsigned int)Always,
    Equal           = (unsigned int)gl::GL_EQUAL,
    LessOrEqual     = (unsigned int)gl::GL_LEQUAL,
    Greater         = (unsigned int)gl::GL_GREATER,
    NotEqual        = (unsigned int)gl::GL_NOTEQUAL,
    GreaterOrEqual  = (unsigned int)gl::GL_GEQUAL
};

enum class DepthWrite
{
    Enabled,
    Disabled
};

class DepthState final
{
public:
    static DepthState enabled();
    static DepthState disabledRW();
    static DepthState disabledR();
    static DepthState disabledW(DepthTest depthTest = DepthTest::Enabled);

public:
    DepthState() = default;
    DepthState(DepthTest depthTest, DepthWrite depthWrite);

    DepthTest depthTest() const;
    DepthWrite depthWrite() const;

    void setDepthTest(DepthTest depthTest);
    void setDepthWrite(DepthWrite depthWrite);

    std::string toString() const;

private:
    DepthTest   m_depthTest = DepthTest::Enabled;
    DepthWrite  m_depthWrite = DepthWrite::Enabled;
};

}

