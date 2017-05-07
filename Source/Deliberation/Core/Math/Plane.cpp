#include <Deliberation/Core/Math/Plane.h>

namespace deliberation
{
Plane::Plane() : m_normal(), m_d(0.0f) {}

Plane::Plane(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c)
{
    // See https://en.wikipedia.org/wiki/Hesse_normal_form
    m_normal = glm::normalize(glm::cross(b - a, c - a));
    if (glm::dot(a, m_normal) < 0)
    {
        m_normal = -m_normal;
    }

    m_d = glm::dot(a, m_normal);
}

const glm::vec3 & Plane::normal() const { return m_normal; }

float Plane::d() const { return m_d; }

void Plane::setNormal(const glm::vec3 & normal) { m_normal = normal; }

void Plane::setD(float d) { m_d = d; }
}
