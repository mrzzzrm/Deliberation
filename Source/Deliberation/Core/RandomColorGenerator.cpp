#include <Deliberation/Core/RandomColorGenerator.h>

#include <Deliberation/Core/Math/Random.h>

namespace
{

const auto GOLDEN_RATIO_CONJUGATE = 0.618033988749895;

}

namespace deliberation
{

RandomColorGenerator::RandomColorGenerator()
{
    m_val = deliberation::RandomFloat(0.0f, 1.0f);
}

glm::vec3 RandomColorGenerator::generate()
{
    m_val += GOLDEN_RATIO_CONJUGATE;
    m_val = fmod(m_val, 1.0f);
    return hsvToRGB({m_val, 0.5f, 0.95f});
}

glm::vec3 RandomColorGenerator::hsvToRGB(const glm::vec3 & hsv) const
{
    auto h = hsv.x;
    auto s = hsv.y;
    auto v = hsv.z;

    glm::vec3 rgb;

    auto h_i = (int)(h * 6.0f);
    auto f = h*6 - h_i;
    auto p = v * (1 - s);
    auto q = v * (1 - f*s);
    auto t = v * (1 - (1 - f) * s);

    switch (h_i)
    {
        case 0: rgb = {v, t, p}; break;
        case 1: rgb = {q, v, p}; break;
        case 2: rgb = {p, v, t}; break;
        case 3: rgb = {p, q, v}; break;
        case 4: rgb = {t, p, v}; break;
        case 5: rgb = {v, p, q}; break;
    }

    return rgb;
}

}
