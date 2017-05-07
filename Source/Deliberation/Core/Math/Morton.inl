#include <Deliberation/Core/BitUtils.h>

namespace deliberation
{
unsigned int Morton2D::NumKeysAtLevel(unsigned int depth)
{
    return 1u << (depth * 2u);
}

Morton2D::Morton2D(uint32_t key) : m_key(key) {}

Morton2D::Morton2D(const glm::uvec2 & cell)
{
    auto x = SeparateBitsBy2(cell.x);
    auto y = SeparateBitsBy2(cell.y);

    m_key = (y << 1) | x;
}

uint32_t Morton2D::toIndex() const { return m_key; }

glm::uvec2 Morton2D::toVec() const
{
    auto x = 0u;
    auto y = 0u;

    for (auto d = 30; d >= 0; d -= 2)
    {
        x <<= 1;
        y <<= 1;

        auto bits = (m_key & (3 << d)) >> d;

        x += bits & 1 ? 1 : 0;
        y += bits & 2 ? 1 : 0;
    }

    return {x, y};
}

unsigned int Morton3D::NumKeysAtLevel(unsigned int depth)
{
    return 1u << (depth * 3u);
}

Morton3D::Morton3D(uint32_t key) : m_key(key) {}

Morton3D::Morton3D(const glm::uvec3 & cell)
{
    auto x = SeparateBitsBy2(cell.x);
    auto y = SeparateBitsBy2(cell.y);
    auto z = SeparateBitsBy2(cell.z);

    m_key = (z << 2) | (y << 1) | x;
}

uint32_t Morton3D::toIndex() const { return m_key; }

glm::uvec3 Morton3D::toVec() const
{
    auto x = 0u;
    auto y = 0u;
    auto z = 0u;

    for (auto d = 27; d >= 0; d -= 3)
    {
        x <<= 1;
        y <<= 1;
        z <<= 1;

        auto bits = (m_key & (7 << d)) >> d;

        x += bits & 1 ? 1 : 0;
        y += bits & 2 ? 1 : 0;
        z += bits & 4 ? 1 : 0;
    }

    return {x, y, z};
}
}
