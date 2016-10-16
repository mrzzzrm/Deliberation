#include <Deliberation/Physics/Contacts/SphereContact.h>

#include <iostream>

#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/SphereShape.h>

namespace deliberation
{

SphereContact::SphereContact(RigidBody & bodyA, RigidBody & bodyB):
    Contact(bodyA, bodyB),
    m_shapeA(*(SphereShape*)bodyA.shape().get()),
    m_shapeB(*(SphereShape*)bodyB.shape().get())
{

}

void SphereContact::update()
{
    auto pA = m_bodyA.transform().position();
    auto pB = m_bodyB.transform().position();

    auto delta = pA - pB;
    auto distance = glm::dot(delta, delta);

    auto threshold = m_shapeA.radius() + m_shapeB.radius();
    threshold = threshold * threshold;

    m_intersect = distance < threshold;

    if (m_intersect)
    {
        Intersection intersection;

        intersection.normal = glm::normalize(pB - pA);
        intersection.position = 0.5f * (pA + pB);
        intersection.depth = glm::dot(pB - pA, intersection.normal) - m_shapeA.radius() - m_shapeB.radius();

        updatePoint(0, intersection);
    }
}

}