#pragma once

#include <functional>

#include <Deliberation/Core/Chrono.h>

namespace deliberation
{
class MainLoop
{
public:
    void run(const std::function<bool(DurationMicros)> & fn);
};
}
