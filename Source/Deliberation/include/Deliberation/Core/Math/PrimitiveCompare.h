#pragma once

#include <Deliberation/Core/Math/Sphere.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

inline bool PrimitivesEqual(const Sphere & lhs, const Sphere & rhs, float tolerance = 0.01f);

}

#include <Deliberation/Core/Math/PrimitiveCompare.inl>