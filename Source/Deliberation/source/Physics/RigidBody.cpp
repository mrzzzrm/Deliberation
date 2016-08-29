#include <Deliberation/Physics/RigidBody.h>

#include <sstream>

#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

RigidBody::RigidBody(const std::shared_ptr<CollisionShape> & shape,
                     const Transform3D & transform,
                     float inverseMass,
                     const glm::vec3 & linearVelocity,
                     const glm::vec3 & angularVelocity):
    CollisionObject(shape, transform),
    m_inverseMass(inverseMass),
    m_linearVelocity(linearVelocity),
    m_angularVelocity(angularVelocity)
{
}

float RigidBody::inverseMass() const
{
    return m_inverseMass;
}

float RigidBody::mass() const
{
    return m_inverseMass != 0.0f ? 1.0f / m_inverseMass : 0.0f;
}

float RigidBody::restitution() const
{
    return m_restitution;
}

const glm::vec3 & RigidBody::linearVelocity() const
{
    return m_linearVelocity;
}

const glm::vec3 & RigidBody::angularVelocity() const
{
    return m_angularVelocity;
}

const glm::vec3 & RigidBody::force() const
{
    return m_force;
}

size_t RigidBody::index() const
{
    return m_index;
}

bool RigidBody::isStatic() const
{
    return m_static;
}

void RigidBody::setInverseMass(float inverseMass)
{
    m_inverseMass = inverseMass;
}

void RigidBody::setRestitution(float restitution)
{
    m_restitution = restitution;
}

void RigidBody::setLinearVelocity(const glm::vec3 & velocity)
{
    m_linearVelocity = velocity;
}

void RigidBody::setAngularVelocity(const glm::vec3 & velocity)
{
    m_angularVelocity = velocity;
}

void RigidBody::setForce(const glm::vec3 & force)
{
    m_force = force;
}

void RigidBody::applyForce(const glm::vec3 & force)
{
    m_force += force;
}

void RigidBody::setStatic(bool isStatic)
{
    m_static = isStatic;
}

void RigidBody::setIndex(size_t index)
{
    m_index = index;
}

void RigidBody::predictTransform(float seconds, Transform3D & prediction)
{
    prediction.setCenter(transform().center());
    prediction.setScale(transform().scale());
    prediction.setPosition(transform().position() + seconds * m_linearVelocity);
    prediction.setOrientation(transform().orientation()/* * glm::quat(seconds * m_angularVelocity)*/);
}

void RigidBody::integrateVelocities(float seconds)
{
    m_linearVelocity += m_force * m_inverseMass * seconds;
    // m_angularVelocity
}

std::string RigidBody::toString() const
{
    std::stringstream stream;
    stream << "{" << transform().position() << ", " << transform().orientation() << "}";
    return stream.str();
}

}

