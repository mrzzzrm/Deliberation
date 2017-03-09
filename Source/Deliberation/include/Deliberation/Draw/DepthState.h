#pragma once

#include <string>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

enum class DepthTest
{
    Less = 0,
    Enabled = Less,
    Always = 1,
    Disabled = Always,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual
};

enum class DepthWrite
{
    Enabled,
    Disabled
};

class DELIBERATION_API DepthState final
{
public:
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

