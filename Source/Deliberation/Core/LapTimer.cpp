#include <Deliberation/Core/LapTimer.h>

namespace deliberation
{
LapTimer::LapTimer() : m_seconds(0.0f)
{
    m_lastLap = std::chrono::steady_clock::now();
}

void LapTimer::lap()
{
    auto now = std::chrono::steady_clock::now();
    m_seconds = std::chrono::duration_cast<std::chrono::duration<float>>(
                    now - m_lastLap)
                    .count();
    m_lastLap = now;
}

float LapTimer::seconds() const { return m_seconds; }
}
