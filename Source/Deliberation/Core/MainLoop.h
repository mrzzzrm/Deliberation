#pragma once

#include <functional>

namespace deliberation
{
class MainLoop
{
public:
    void run(const std::function<bool(DurationMicros)> & fn);
};
}
