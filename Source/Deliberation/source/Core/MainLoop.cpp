#include <Deliberation/Core/MainLoop.h>

#include <chrono>

namespace deliberation
{

void MainLoop::run(const std::function<bool(float)> & fn)
{
    float lastFrameSeconds = 0.0f;

    while (true)
    {
        auto before = std::chrono::steady_clock::now();

        auto resume = fn(lastFrameSeconds);
        if (!resume)
        {
            break;
        }

        auto after = std::chrono::steady_clock::now();

        auto dur = after - before;
        lastFrameSeconds = std::chrono::duration_cast<std::chrono::duration<float>>(dur).count();
    }
}

}

