#pragma once

#include <cstddef>



namespace deliberation
{

template<typename T, typename _T, typename M>
std::ptrdiff_t OffsetOf(const T & t, M _T::* ptr);

}

#include <Deliberation/Core/OffsetOf.inl>
