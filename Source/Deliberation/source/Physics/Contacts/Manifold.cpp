#include <Deliberation/Physics/Contacts/Manifold.h>

namespace deliberation
{

Manifold::Manifold()
{

}

Manifold::Manifold(float depth,
                   const glm::vec3 & position,
                   const glm::vec3 & localPositionA,
                   const glm::vec3 & localPositionB,
                   const glm::vec3 & normal):
    m_depth(depth),
    m_position(position),
    m_localPositionA(localPositionA),
    m_localPositionB(localPositionB),
    m_normal(normal)
{

}

float Manifold::depth() const
{
    return m_depth;
}

const glm::vec3 & Manifold::position() const
{
    return m_position;
}

const glm::vec3 & Manifold::localPositionA() const
{
    return m_localPositionA;
}

const glm::vec3 & Manifold::localPositionB() const
{
    return m_localPositionB;
}

const glm::vec3 & Manifold::normal() const
{
    return m_normal;
}

void Manifold::setDepth(float depth)
{
    m_depth = depth;
}

void Manifold::setPosition(const glm::vec3 & position)
{
    m_position = position;
}

void Manifold::setLocalPointA(const glm::vec3 & localPositionA)
{
    m_localPositionA = localPositionA;
}

void Manifold::setLocalPointB(const glm::vec3 & localPositionB)
{
    m_localPositionB = localPositionB;
}

void Manifold::setNormal(const glm::vec3 & normal)
{
    m_normal = normal;
}

}