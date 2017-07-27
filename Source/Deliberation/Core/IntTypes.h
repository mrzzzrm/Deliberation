#pragma once

#include <cstdint>
#include <limits>

namespace deliberation
{
using i8 = int8_t;
using u8 = uint8_t;
using i16 = int16_t;
using u16 = uint16_t;
using i32 = int32_t;
using u32 = uint32_t;
using i64 = int64_t;
using u64 = uint64_t;
using uint = u32;

constexpr std::size_t INVALID_SIZE_T = std::numeric_limits<std::size_t>::max();
}
