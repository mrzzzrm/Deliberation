#pragma once

#include <Deliberation/Core/Assert.h>

namespace deliberation
{
template<typename T>
i32 SigNum(T value)
{
    return (T(0) < value) - (value < T(0));
}

template<typename T>
i32 Sign(T value)
{
    return value >= T(0) ? T(1) : T(-1);
}

float Lerp(float a, float b, float w)
{
    Assert(0.0f <= w && 1.0f >= w, "");
    return a * (1 - w) + b * w;
}
}