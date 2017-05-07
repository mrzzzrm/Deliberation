#include <Deliberation/Core/Math/Sphere.h>

#include <cassert>

#include <Deliberation/Core/Math/AABB.h>

namespace deliberation
{
Sphere Sphere::containingAABB(const AABB & aabb)
{
    return Sphere(
        (aabb.llf() + aabb.urb()) / 2.0f,
        glm::length(aabb.urb() - aabb.llf()) / 2.0f);
}

Sphere::Sphere() : m_radius(0.0f) {}

Sphere::Sphere(const glm::vec3 & position, float radius)
    : m_position(position), m_radius(radius)
{
    assert(radius >= 0);
}

float Sphere::radius() const { return m_radius; }

void Sphere::setRadius(float radius)
{
    assert(radius >= 0);
    m_radius = radius;
}

const glm::vec3 & Sphere::position() const { return m_position; }

void Sphere::setPosition(const glm::vec3 & position) { m_position = position; }

bool Sphere::intersects(const Sphere & other) const
{
    auto      sumRadii = other.m_radius + m_radius;
    glm::vec3 delta = other.m_position - m_position;

    return glm::dot(delta, delta) <= sumRadii * sumRadii;
}
}
