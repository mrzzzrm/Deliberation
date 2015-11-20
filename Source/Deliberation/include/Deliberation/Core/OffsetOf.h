#pragma once

#include <cstddef>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T, typename _T, typename M>
std::ptrdiff_t DELIBERATION_API OffsetOf(const T & t, M _T::* ptr);

}

#include <Deliberation/Core/OffsetOf.inl>
