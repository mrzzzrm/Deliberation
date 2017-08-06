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
        DurationMicros frameDuration;
        TimestampMicros newTimestamp;

        do { // Hotloop until some time has passed
            newTimestamp = CurrentMicros();
            frameDuration = newTimestamp - timestamp;
        } while (frameDuration < 200);

        timestamp = newTimestamp;

        auto resume = fn(frameDuration);
        if (!resume) break;
    }
}
}
