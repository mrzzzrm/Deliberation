namespace deliberation
{

bool EpsilonEQ(const float a, const float b)
{
    return std::abs(a - b) < FLOAT_COMPARE_EPSILON;
}

bool EpsilonGT(const float a, const float b)
{
    return a - b > FLOAT_COMPARE_EPSILON;
}

}