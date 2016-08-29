#include <Deliberation/Physics/Contacts/Contact.h>

namespace deliberation
{

Contact::Contact(RigidBody & bodyA, RigidBody & bodyB):
    m_bodyA(bodyA),
    m_bodyB(bodyB)
{

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

const glm::vec3 & Contact::localPointA() const
{
    return m_localPointA;
}

const glm::vec3 & Contact::localPointB() const
{
    return m_localPointB;
}

const glm::vec3 & Contact::normal() const
{
    return m_normal;
}

float Contact::normalMass() const
{
    return m_normalMass;
}

float Contact::velocityBias() const
{
    return m_velocityBias;
}

float Contact::normalImpulse() const
{
    return m_normalImpulse;
}

float Contact::separation() const
{
    return m_separation;
}

void Contact::setNormalImpulse(float normalImpulse)
{
    m_normalImpulse = normalImpulse;
}

}