#include <Deliberation/Physics/Contacts/Contact.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

void ContactPoint::initialize(const Contact & contact, const Intersection & intersection)
{
    update(contact, intersection);

    normalImpulseAccumulator = 0.0f;
}

void ContactPoint::update(const Contact & contact, const Intersection & intersection)
{
    *(Intersection*)this = intersection;

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto & n = normal;

    auto mA = bodyA.inverseMass();
    auto & vA = bodyA.linearVelocity();
    auto & wA = bodyA.angularVelocity();
    auto & iA = bodyA.worldInverseInertia();

    auto mB = bodyB.inverseMass();
    auto & vB = bodyB.linearVelocity();
    auto & wB = bodyB.angularVelocity();
    auto & iB = bodyB.worldInverseInertia();

    relativePositionA = position - bodyA.transform().position();
    relativePositionB = position - bodyB.transform().position();

    auto vContactA = vA + glm::cross(wA, relativePositionA);
    auto vContactB = vB + glm::cross(wB, relativePositionB);

    auto vRelProjected = glm::dot(normal, vContactB - vContactA);

    if (vRelProjected > 0)
    {
        velocityBias = -vRelProjected * contact.restitution();
    }
    else
    {
        velocityBias = 0;
    }

    normalMass = glm::dot(glm::cross(iA * glm::cross(relativePositionA, n), relativePositionA) +
                          glm::cross(iB * glm::cross(relativePositionB, n), relativePositionB), n) + mA + mB;
}

Contact::Contact(RigidBody & bodyA, RigidBody & bodyB):
    m_bodyA(bodyA),
    m_bodyB(bodyB),
    m_intersect(false),
    m_numPoints(0)
{
    m_resitution = std::max(m_bodyA.restitution(), m_bodyB.restitution());
}

Contact::~Contact() = default;

RigidBody & Contact::bodyA() const
{
    return m_bodyA;
}

RigidBody & Contact::bodyB() const
{
    return m_bodyB;
}

bool Contact::intersect() const
{
    return m_intersect;
}

float Contact::restitution() const
{
    return m_resitution;
}

uint Contact::numPoints() const
{
    return m_numPoints;
}

ContactPoint & Contact::point(uint index)
{
    Assert(index < m_numPoints, "");
    return m_points[index];
}

int Contact::matchPoint(const glm::vec3 & position)
{
    for (uint p = 0; p < m_numPoints; p++)
    {
        auto * point = &m_points[p];
        if (glm::length(position - point->position) < 0.2f)
        {
            return p;
        }
    }
    return -1;
}

void Contact::updatePoint(uint index, const Intersection & intersection)
{
    Assert(index < m_numPoints, "");

    auto * point = &m_points[index];
    point->update(*this, intersection);
}

void Contact::addPoint(const Intersection & intersection)
{
    if (m_numPoints < MAX_NUM_CONTACT_POINTS)
    {
        m_points[m_numPoints].initialize(*this, intersection);
        m_numPoints++;
    }
}

}