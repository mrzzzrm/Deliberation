#pragma once

#include <string>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class ScopeProfiler {
public:
    ScopeProfiler(const std::string & name);
    ~ScopeProfiler();

private:
    std::string         m_name;
    TimestampMicros     m_start;
};

}