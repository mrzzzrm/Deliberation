namespace deliberation
{

bool EpsilonEq(const float a, const float b)
{
    return std::abs(a - b) < FLOAT_COMPARE_EPSILON;
}

bool EpsilonGt(const float a, const float b)
{
    return a - b > FLOAT_COMPARE_EPSILON;
}

}