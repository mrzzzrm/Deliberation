#include <Deliberation/Core/Scope.h>

#include <algorithm>

#include <Deliberation/Core/ScopeStack.h>

namespace deliberation
{

Scope::Scope(const std::string & name):
    m_name(name)
{}

DurationMicros Scope::minDuration() const
{
    if (m_durations.empty()) return 0;
    return *std::min_element(m_durations.begin(), m_durations.end());
}

DurationMicros Scope::maxDuration() const
{
    if (m_durations.empty()) return 0;
    return *std::max_element(m_durations.begin(), m_durations.end());
}

DurationMicros Scope::medianDuration() const
{
    if (m_durations.empty()) return 0;
    return m_durationsHalfSorted[m_durationsHalfSorted.size() / 2];
}

void Scope::begin()
{
    AssertM(m_beginTimestamp == 0, "begin() called twice()");
    m_beginTimestamp = CurrentMicros();
}

void Scope::end()
{
    AssertM(m_beginTimestamp != 0, "end() called without begin()");
    addInvocation(CurrentMicros() - m_beginTimestamp);
    m_beginTimestamp = 0;
}

void Scope::addInvocation(DurationMicros duration)
{
    m_durations.emplace_back(duration);

    if (m_durations.size() > MAX_NUM_CACHED) m_durations.pop_front();

    m_durationsHalfSorted.resize(m_durations.size());
    std::copy(m_durations.begin(), m_durations.end(), m_durationsHalfSorted.begin());
    std::nth_element(m_durationsHalfSorted.begin(),
                     m_durationsHalfSorted.begin() + m_durationsHalfSorted.size()/2,
                     m_durationsHalfSorted.end());

}

ScopeRAII::ScopeRAII(const char *name)
{
    GetGlobal<ScopeStack>()->pushScope(name);
}

ScopeRAII::~ScopeRAII()
{
    GetGlobal<ScopeStack>()->popScope();
}

}