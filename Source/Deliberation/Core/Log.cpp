#include <Deliberation/Core/Log.h>

#include <Deliberation/Core/IntTypes.h>

namespace
{

std::string InnerScope;
std::string OuterScope;
uint32_t FrameIndex = 0;

}

namespace deliberation
{

std::shared_ptr<spdlog::logger> Log;

void LogSetInnerScope(const char * name)
{
    InnerScope = name;
    LogUpdatePattern();
}

void LogSetOuterScope(const char * name)
{
    OuterScope = name;
    LogUpdatePattern();
}

void LogSetFrameIndex(uint32_t frameIndex)
{
    FrameIndex = frameIndex;
    LogUpdatePattern();
}

void LogUpdatePattern()
{
    spdlog::set_pattern(fmt::format("[%H:%M:%S] [{}-{}/{}] %v", FrameIndex, OuterScope, InnerScope));
}

}