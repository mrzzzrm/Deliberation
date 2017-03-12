#pragma once

#include <vector>

#include <Deliberation/Core/Chrono.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class DELIBERATION_API FpsCounter
{
public:
    static constexpr size_t BACKLOG_CAPACITY = 15;

public:
    float fps() const;
    void onFrame();

private:
    std::vector<TimestampMillis> m_backlog;
};

}