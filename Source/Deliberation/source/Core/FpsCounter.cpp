#include <Deliberation/Core/FpsCounter.h>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/FloatUtils.h>

namespace deliberation
{

float FpsCounter::fps() const
{
    const auto numFrames = m_backlog.size();

    if (numFrames < 2) return 0.0f;

    const auto duration = m_backlog.back() - m_backlog.front();
    Assert(EpsilonGt(duration, 0.0f), "");

    return 1000.0f / (duration / numFrames);
}

void FpsCounter::onFrame()
{
    m_backlog.emplace_back(CurrentMillis());

    while (m_backlog.size() > BACKLOG_CAPACITY) m_backlog.erase(m_backlog.begin());
}

}