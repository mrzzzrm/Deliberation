#pragma once

#include <chrono>

namespace deliberation
{
class LapTimer final
{
  public:
    LapTimer();

    void  lap();
    float seconds() const;

  private:
    std::chrono::steady_clock::time_point m_lastLap;
    float                                 m_seconds;
};
}
