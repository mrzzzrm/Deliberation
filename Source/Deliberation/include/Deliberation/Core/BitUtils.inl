#pragma once

namespace deliberation
{

template<typename T>
unsigned int LeftmostBit(const T & value)
{
    for (int testIndex = (sizeof(T) * 8 - 1);
         testIndex >= 0;
         testIndex--)
    {
        if (value & (1 << testIndex))
        {
            return testIndex;
        }
    }

    return ~0;
}

uint32_t SeparateBitsBy2(uint32_t i)
{
    i = (i ^ (i << 16)) & 0xFF0000FF;
    i = (i ^ (i <<  8)) & 0x0300F00F;
    i = (i ^ (i <<  4)) & 0x030C30C3;
    i = (i ^ (i <<  2)) & 0x09249249;

    return i;
}

}

