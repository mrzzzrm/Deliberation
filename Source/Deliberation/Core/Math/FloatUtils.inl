#include <cmath>

namespace deliberation
{
bool EpsilonEq(const float a, const float b)
{
    return std::fabs(a - b) < FLOAT_COMPARE_EPSILON;
}

bool EpsilonGt(const float a, const float b)
{
    return a - b > FLOAT_COMPARE_EPSILON;
}

template<typename T, glm::precision P>
bool EpsilonEq(const glm::tvec3<T, P> & a, const glm::tvec3<T, P> & b)
{
    return EpsilonEq(a.x, b.x) && EpsilonEq(a.y, b.y) && EpsilonEq(a.z, b.z);
};
}