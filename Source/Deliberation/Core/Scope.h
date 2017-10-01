#pragma once

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{

class Scope final
{
public:
    Scope(const std::string & name);

    DurationMicros minDuration();
    DurationMicros maxDuration();
    DurationMicros medianDuration();

    void addInvocation(DurationMicros duration);

private:
    std::string m_name;
    std::vector<DurationMicros> m_durations;
};

}