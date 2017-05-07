#include <Deliberation/Core/BitUtils.h>

namespace deliberation
{
template<>
unsigned int LeftmostBit(const uint32_t & value)
{
    auto x = value;

    // http://stackoverflow.com/a/10273678
    static const unsigned int bval[] = {
        0, 1, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4};

    unsigned int r = 0;
    if (x & 0xFFFF0000)
    {
        r += 16 / 1;
        x >>= 16 / 1;
    }
    if (x & 0x0000FF00)
    {
        r += 16 / 2;
        x >>= 16 / 2;
    }
    if (x & 0x000000F0)
    {
        r += 16 / 4;
        x >>= 16 / 4;
    }
    return r + bval[x];
}
}
