#pragma once

#include <spdlog/spdlog.h>

#include <Deliberation/Core/Format.h>

namespace deliberation
{

extern std::shared_ptr<spdlog::logger> Log;

void LogSetInnerScope(const char * name);
void LogSetOuterScope(const char * name);
void LogSetFrameIndex(uint32_t frameIndex);
void LogUpdatePattern();

struct LogInnerScope final
{
    LogInnerScope(const char * name) { LogSetInnerScope(name); }
    ~LogInnerScope() { LogSetInnerScope(""); }
};

struct LogOuterScope final
{
    LogOuterScope(const char * name) { LogSetOuterScope(name); }
    ~LogOuterScope() { LogSetOuterScope(""); }
};

}

#define DELIBERATION_LOG_OUTER_SCOPE(name) deliberation::LogOuterScope logOuterScope(name);
#define DELIBERATION_LOG_INNER_SCOPE(name) deliberation::LogInnerScope logInnerScope(name);
