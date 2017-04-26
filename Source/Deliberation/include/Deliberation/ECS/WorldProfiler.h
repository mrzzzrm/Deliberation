#pragma once

#include <vector>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class SystemBase;

class WorldProfilerScope final
{
public:
    WorldProfilerScope(SystemBase & system,
                       const std::string & phase,
                       DurationMicros micros);

    std::string toString() const;

    bool operator<(const WorldProfilerScope & rhs) const;

private:
    SystemBase *        m_system;
    std::string         m_phase;
    DurationMicros      m_micros;
};

class WorldProfiler final
{
public:
    const std::vector<WorldProfilerScope> & scopes() const;

    void addScope(const WorldProfilerScope & scope);

    void frameComplete();

private:
    std::vector<WorldProfilerScope> m_currentFrameScopes;
    std::vector<WorldProfilerScope> m_lastFrameScopes;
};

}
