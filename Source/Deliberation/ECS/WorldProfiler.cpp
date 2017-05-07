#include <Deliberation/ECS/WorldProfiler.h>

#include <Deliberation/ECS/SystemBase.h>

#include <algorithm>

namespace deliberation
{
WorldProfilerScope::WorldProfilerScope(
    SystemBase & system, const std::string & phase, DurationMicros micros)
    : m_system(&system), m_phase(phase), m_micros(micros)
{
}

std::string WorldProfilerScope::toString() const
{
    return m_system->name() + "::" + m_phase + ": " + std::to_string(m_micros);
}

bool WorldProfilerScope::operator<(const WorldProfilerScope & rhs) const
{
    return m_micros > rhs.m_micros;
}

const std::vector<WorldProfilerScope> & WorldProfiler::scopes() const
{
    return m_lastFrameScopes;
}

void WorldProfiler::addScope(const WorldProfilerScope & scope)
{
    m_currentFrameScopes.emplace_back(scope);
}

void WorldProfiler::frameComplete()
{
    std::swap(m_currentFrameScopes, m_lastFrameScopes);
    std::sort(m_lastFrameScopes.begin(), m_lastFrameScopes.end());
    m_currentFrameScopes.clear();
}
}
