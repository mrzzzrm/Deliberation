#include <Deliberation/Core/Math/Box.h>

namespace deliberation
{

Box::Box()
{

}

Box::Box(const glm::vec3 & p, const glm::vec3 & x, const glm::vec3 & y, const glm::vec3 & z):
    m_p(p),
    m_x(x),
    m_y(y),
    m_z(z)
{

}

const glm::vec3 & Box::p() const
{
    return m_p;
}

const glm::vec3 & Box::x() const
{
    return m_x;
}

const glm::vec3 & Box::y() const
{
    return m_y;
}

const glm::vec3 & Box::z() const
{
    return m_z;
}

AABB Box::bounds() const
{
    auto v = m_x + m_y + m_z;
    auto halfExtent = glm::vec3(v, v, v);
    return {m_p - halfExtent, m_p + halfExtent};
}

}

