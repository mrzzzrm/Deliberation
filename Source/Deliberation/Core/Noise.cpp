#include <Deliberation/Core/Noise.h>

#include <iostream>

#include <Deliberation/Core/Math/MathUtils.h>

namespace deliberation
{

float IntegerNoise(i32 v)
{
    v = (v >> 13) ^ v;
    auto vv = (v * (v * v * 60493 + 19990303) + 1376312589) & 0x7fffffff;
    return (float)(1.0 - ((double)vv / 1073741824.0));
}

float CoherentNoise(float v)
{
    auto a = IntegerNoise((i32)v);
    auto b = IntegerNoise((i32)v + 1);

    return Lerp(a, b, v - floor(v));
}

}