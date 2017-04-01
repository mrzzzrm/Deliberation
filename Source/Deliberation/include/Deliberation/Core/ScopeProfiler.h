#pragma once

#include <string>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class ScopeProfiler {
public:
    ScopeProfiler(const std::string & name = {});
    ~ScopeProfiler();

    DurationMicros stop();

private:
    std::string         m_name;
    TimestampMicros     m_start;
    bool                m_stopped = false;
};

}