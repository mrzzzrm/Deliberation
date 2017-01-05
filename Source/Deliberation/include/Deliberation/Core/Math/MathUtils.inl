#pragma once

namespace deliberation
{

template <typename T> i32 SigNum(T value)
{
    return (T(0) < value) - (value < T(0));
}

}