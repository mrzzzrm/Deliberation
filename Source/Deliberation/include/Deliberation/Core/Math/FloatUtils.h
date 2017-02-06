#pragma once

#include <Deliberation/Deliberation.h>

namespace deliberation
{

constexpr float FLOAT_COMPARE_EPSILON = 0.0001f;

inline bool DELIBERATION_API EpsilonEQ(const float a, const float b);
inline bool DELIBERATION_API EpsilonGT(const float a, const float b);

}

#include <Deliberation/Core/Math/FloatUtils.inl>