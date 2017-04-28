#include <Deliberation/Physics/Contacts/Contact.h>

#include <algorithm>
#include <iostream>
#include <string.h>

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

    if (vRelProjected < 0)
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
    m_friction = (m_bodyA.friction() + m_bodyB.friction()) / 2.0f;
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

float Contact::friction() const
{
    return m_friction;
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

void Contact::updatePoints(const Span<Intersection> & intersections)
{
    std::sort(intersections.begin(), intersections.end(), [](const Intersection & a, const Intersection & b) {
        return a.depth > b.depth;
    });

    auto numConsideredIntersections = std::min<uint>(intersections.size(), MAX_NUM_CONTACT_POINTS);

    ContactPoint newPoints[MAX_NUM_CONTACT_POINTS];


    for (auto i = 0; i < numConsideredIntersections; i++)
    {
        uint p = 0;
        for (; p < m_numPoints; p++)
        {
            if (intersectionMatchesPoint(intersections[i], m_points[p]))
            {
                newPoints[i] = m_points[p];
                newPoints[i].update(*this, intersections[i]);
                break;
            }
        }

        if (p == m_numPoints)
        {
            std::cout << "New ContactPoint" << std::endl;

            newPoints[i].initialize(*this, intersections[i]);
        }
    }

    memcpy(m_points, newPoints, numConsideredIntersections * sizeof(ContactPoint));
    m_numPoints = numConsideredIntersections;
}

bool Contact::intersectionMatchesPoint(const Intersection & intersection, const ContactPoint & point) const
{
    return glm::length(intersection.position - point.position) < 0.3f;
}

void Contact::clearPoints()
{
    m_numPoints = 0;
}



}