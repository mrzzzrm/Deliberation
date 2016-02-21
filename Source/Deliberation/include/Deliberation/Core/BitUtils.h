#pragma once

#include <stdint.h>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

template<typename T>
unsigned int DELIBERATION_API LeftmostBit(const T & value);

template<>
unsigned int DELIBERATION_API LeftmostBit(const uint32_t & value);

inline uint32_t DELIBERATION_API SeparateBitsBy2(uint32_t value);

}

#include "BitUtils.inl"
