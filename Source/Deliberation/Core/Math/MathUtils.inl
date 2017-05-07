#pragma once

namespace deliberation
{
template<typename T>
i32 SigNum(T value)
{
    return (T(0) < value) - (value < T(0));
}

template<typename T>
i32 Sign(T value)
{
    return value >= T(0) ? T(1) : T(-1);
}
}