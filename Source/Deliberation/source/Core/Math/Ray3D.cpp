#include <Deliberation/Core/Math/Ray3D.h>

#include <cmath>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{

Ray3D Ray3D::fromTo(const glm::vec3 & from, const glm::vec3 & to)
{
    return Ray3D(from, to - from);
}

Ray3D::Ray3D(const glm::vec3 & origin, const glm::vec3 & direction):
    m_origin(origin),
    m_direction(direction)
{
    Assert(glm::length(direction) != 0, "");
}

const glm::vec3 & Ray3D::origin() const
{
    return m_origin;
}

const glm::vec3 & Ray3D::direction() const
{
    return m_direction;
}

void Ray3D::setOrigin(const glm::vec3 & origin)
{
    m_origin = origin;
}

void Ray3D::setDirection(const glm::vec3 & direction)
{
    Assert(glm::length(direction) != 0, "");
    m_direction = direction;
}

//bool Ray3D::intersects(const Sphere & sphere) const
//{
//    glm::vec3 o = m_origin - sphere.position();
//
//    float a = glm::dot(m_direction, m_direction);
//    float b = 2 * glm::dot(m_direction, o);
//    float c = glm::dot(o, o) - (sphere.radius() * sphere.radius());
//
//    float t0 = (-b - std::sqrt(b*b - 4*a*c)) / (2*a);
//    float t1 = (-b + std::sqrt(b*b - 4*a*c)) / (2*a);
//
//    return t0 >= 0 || t1 >= 0;
//}

Ray3D Ray3D::normalized() const
{
    Ray3D r(*this);
    r.normalize();
    return r;
}

void Ray3D::normalize()
{
    m_direction = glm::normalize(m_direction);
}

glm::vec3 Ray3D::at(float s) const
{
    return m_origin + s * m_direction;
}

}

