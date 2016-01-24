#pragma once

#include <chrono>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API LapTimer final
{
public:
    LapTimer();

    void lap();
    float seconds() const;

private:
    std::chrono::steady_clock::time_point   m_lastLap;
    float                                   m_seconds;
};

}
