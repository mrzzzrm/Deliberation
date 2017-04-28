#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Assert.h>

namespace deliberation
{

Box::Box()
{

}

Box::Box(const glm::vec3 & p, const glm::vec3 & x, const glm::vec3 & y, const glm::vec3 & z):
    m_p(p),
    m_axes(glm::normalize(x), glm::normalize(y), glm::normalize(z))
{
    m_halfExtent = glm::vec3(glm::length(x), glm::length(y), glm::length(z));
}

Box::Box(const glm::vec3 & halfExtent, const Transform3D & transform)
{
    m_p = transform.position();
    auto x = transform.directionLocalToWorld(glm::vec3(halfExtent.x, 0.0f, 0.0f));
    auto y = transform.directionLocalToWorld(glm::vec3(0.0f, halfExtent.y, 0.0f));
    auto z = transform.directionLocalToWorld(glm::vec3(0.0f, 0.0f, halfExtent.z));

    x = x != glm::vec3(0.0f) ? glm::normalize(x) : x;
    y = y != glm::vec3(0.0f) ? glm::normalize(y) : y;
    z = z != glm::vec3(0.0f) ? glm::normalize(z) : z;

    m_halfExtent = halfExtent;
    m_axes = glm::mat3(x, y, z);
}

const glm::vec3 & Box::p() const
{
    return m_p;
}

const glm::vec3 & Box::x() const
{
    return m_axes[0];
}

const glm::vec3 & Box::y() const
{
    return m_axes[1];
}

const glm::vec3 & Box::z() const
{
    return m_axes[2];
}

const glm::vec3 & Box::halfExtent() const
{
    return m_halfExtent;
}

const glm::mat3 & Box::orientationMatrix() const
{
    return m_axes;
}

const glm::vec3 & Box::axis(size_t i) const
{
    Assert(i < 3, "");
    return m_axes[i];
}

AABB Box::bounds() const
{
    auto halfExtent = m_halfExtent.x + m_halfExtent.y + m_halfExtent.z;
    return {m_p - halfExtent, m_p + halfExtent};
}

glm::vec3 Box::point(size_t index) const
{
    Assert(index < 8, "");
    
    auto x = m_halfExtent.x * this->x();
    auto y = m_halfExtent.y * this->y();
    auto z = m_halfExtent.z * this->z();

    switch(index)
    {
        case 0: return m_p + x + y + z;
        case 1: return m_p - x + y + z;
        case 2: return m_p + x - y + z;
        case 3: return m_p - x - y + z;
        case 4: return m_p + x + y - z;
        case 5: return m_p - x + y - z;
        case 6: return m_p + x - y - z;
        case 7: return m_p - x - y - z;
    }

    return {};
}

}

