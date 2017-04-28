#pragma once

namespace deliberation
{

template<typename T, typename _T, typename M>
std::ptrdiff_t OffsetOf(const T & t, M _T::* ptr)
{
    return reinterpret_cast<const char*>(&(t.*ptr)) - reinterpret_cast<const char*>(&t);
}

}

