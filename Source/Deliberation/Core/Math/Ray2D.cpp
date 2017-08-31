#include <Deliberation/Core/Math/Ray2D.h>

#include <cmath>

#include <Deliberation/Core/StreamUtils.h>
#include <Deliberation/Core/Assert.h>

#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{
Ray2D Ray2D::fromTo(const glm::vec2 & from, const glm::vec2 & to)
{
    return Ray2D(from, to - from);
}

Ray2D::Ray2D(const glm::vec2 & origin, const glm::vec2 & direction)
    : m_origin(origin), m_direction(direction)
{
    Assert(glm::length(direction) != 0);
}

const glm::vec2 & Ray2D::origin() const { return m_origin; }

const glm::vec2 & Ray2D::direction() const { return m_direction; }

void Ray2D::setOrigin(const glm::vec2 & origin) { m_origin = origin; }

void Ray2D::setDirection(const glm::vec2 & direction)
{
    Assert(glm::length(direction) != 0);
    m_direction = direction;
}

Ray2D Ray2D::normalized() const
{
    Ray2D r(*this);
    r.normalize();
    return r;
}

void Ray2D::normalize() { m_direction = glm::normalize(m_direction); }

glm::vec2 Ray2D::at(float s) const { return m_origin + s * m_direction; }
}

std::ostream & operator<<(std::ostream & os, const deliberation::Ray2D & r)
{
    os << "{" << r.origin() << " -> " << r.direction() << "}";
    return os;
}
