#pragma once

#include <deque>

#include <Deliberation/Core/Chrono.h>

namespace deliberation {

class Scope final {
public:
    static constexpr size_t MAX_NUM_CACHED = 30;

public:
    explicit Scope(const std::string &name);

    const std::string &name() const { return m_name; }

    DurationMicros minDuration() const;

    DurationMicros maxDuration() const;

    DurationMicros medianDuration() const;

    void begin();

    void end();

    void addInvocation(DurationMicros duration);

private:
    std::string m_name;
    std::deque<DurationMicros> m_durations;
    std::vector<DurationMicros> m_durationsHalfSorted;
    TimestampMicros m_beginTimestamp = 0;
};

class ScopeRAII final {
public:
    ScopeRAII(const char *name);

    ~ScopeRAII();
};

}

#define DELIBERATION_SCOPE(name) \
    ScopeRAII scopeRAII(name);