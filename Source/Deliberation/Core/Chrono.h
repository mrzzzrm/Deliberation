#pragma once

#include <chrono>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
using TimestampMicros = u64;
using TimestampMillis = u64;
using DurationMicros = u64;
using DurationMillis = u64;

TimestampMicros CurrentMicros();
TimestampMillis CurrentMillis();

constexpr TimestampMicros INVALID_TIMESTAMP_MICROS =
    std::numeric_limits<TimestampMicros>::max();
constexpr TimestampMillis INVALID_TIMESTAMP_MILLIS =
    std::numeric_limits<TimestampMillis>::max();
constexpr DurationMicros INVALID_DURATION_MILLIS =
    std::numeric_limits<DurationMicros>::max();
constexpr DurationMillis INVALID_DURATION_MICROS =
    std::numeric_limits<DurationMillis>::max();
}

#include <Deliberation/Core/Chrono.inl>