#include <Deliberation/Core/MainLoop.h>

#include <Deliberation/Core/Chrono.h>

#include <algorithm>
#include <chrono>

namespace deliberation
{
void MainLoop::run(const std::function<bool(DurationMicros)> & fn)
{
    TimestampMicros timestamp = CurrentMicros();

    while (true)
    {
        const auto newTimestamp = CurrentMicros();
        const auto frameDuration = newTimestamp - timestamp;
        timestamp = newTimestamp;

        auto resume = fn(frameDuration);
        if (!resume) break;
    }
}
}
