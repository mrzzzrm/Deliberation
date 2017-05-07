#pragma once

#include <stdint.h>

namespace deliberation
{
template<typename T>
unsigned int LeftmostBit(const T & value);

template<>
unsigned int LeftmostBit(const uint32_t & value);

inline uint32_t SeparateBitsBy2(uint32_t value);
}

#include "BitUtils.inl"
