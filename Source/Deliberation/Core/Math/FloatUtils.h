#pragma once

#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Deliberation.h>

namespace deliberation
{
constexpr float FLOAT_COMPARE_EPSILON = 0.0001f;

inline bool EpsilonEq(const float a, const float b);
inline bool EpsilonGt(const float a, const float b);

template<typename T, glm::precision P>
inline bool EpsilonEq(const glm::tvec3<T,P> & a, const glm::tvec3<T,P> & b);

}

#include <Deliberation/Core/Math/FloatUtils.inl>