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

}

#include <Deliberation/Core/Chrono.inl>