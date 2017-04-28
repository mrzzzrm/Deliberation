#include <Deliberation/Core/Math/Rect3D.h>

namespace deliberation
{

Rect3D::Rect3D(const glm::vec3 & origin,
               const glm::vec3 & right,
               const glm::vec3 & up):
    m_origin(origin),
    m_right(right),
    m_up(up)
{

}

const glm::vec3 & Rect3D::origin() const
{
    return m_origin;
}

const glm::vec3 & Rect3D::right() const
{
    return m_right;
}

const glm::vec3 & Rect3D::up() const
{
    return m_up;
}

glm::vec2 Rect3D::size() const
{
    return {glm::length(m_right), glm::length(m_up)};
}

void Rect3D::setOrigin(const glm::vec3 & origin)
{
    m_origin = origin;
}

void Rect3D::setRight(const glm::vec3 & right)
{
    m_right = right;
}

void Rect3D::setUp(const glm::vec3 & up)
{
    m_up = up;
}

Plane Rect3D::plane() const
{
    return Plane(m_origin, m_origin + m_up, m_origin + m_right);
}

}

