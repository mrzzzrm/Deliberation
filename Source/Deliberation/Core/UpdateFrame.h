#pragma once

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{

/**
 * Details about timings in one simulated gameplay frame
 */
class UpdateFrame final
{
public:
    TimestampMicros beginMicros() const { return m_beginMicros; }
    TimestampMicros endMicros() const { return m_beginMicros + m_gameMicros; }
    DurationMicros gameMicros() const { return m_gameMicros; }
    float gameSeconds() const { return m_gameSeconds; }
    float physicsSeconds() const { return m_physicsSeconds; }

    void setBeginMicros(TimestampMicros beginMicros) { m_beginMicros = beginMicros; }
    void setGameMicros(DurationMicros gameMicros) {
        m_gameMicros = gameMicros;
        m_gameSeconds = (double)gameMicros / 1000000.0; // Not sure if floats would be sufficient here
    }
    void setPhysicsSeconds(float physicsSeconds) {
        m_physicsSeconds = physicsSeconds;
    }

private:
    TimestampMicros m_beginMicros = 0;
    DurationMicros m_gameMicros = 0;
    float m_gameSeconds = 0.0f;
    float m_physicsSeconds = 0.0f;
};

}