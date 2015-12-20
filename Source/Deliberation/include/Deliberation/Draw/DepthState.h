#pragma once

#include <string>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API DepthState final
{
public:
    DepthState();
    DepthState(bool depthTestEnabled, bool depthMask);

    bool depthTest() const;
    bool depthMask() const;

    void setDepthTest(bool enabled);
    void setDepthMask(bool mask);

    std::string toString() const;

private:
    bool m_depthTestEnabled;
    bool m_depthMask;
};

}

