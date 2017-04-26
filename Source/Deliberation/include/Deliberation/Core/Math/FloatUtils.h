#pragma once

#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

constexpr float FLOAT_COMPARE_EPSILON = 0.0001f;

inline bool EpsilonEq(const float a, const float b);
inline bool EpsilonGt(const float a, const float b);

}

#include <Deliberation/Core/Math/FloatUtils.inl>