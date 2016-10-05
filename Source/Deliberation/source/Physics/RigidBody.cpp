#include <Deliberation/Physics/RigidBody.h>

#include <sstream>

#include <Deliberation/Core/Math/MathUtils.h>
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
    auto localInertia = shape->localInertia() * mass();
    m_localInvInertia = InverseDiagonalMatrix(localInertia);

    updateWorldInertia();
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

const glm::mat3 & RigidBody::worldInvInertia() const
{
    return m_worldInvInertia;
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
    if (m_static)
    {
        return;
    }

    m_linearVelocity = velocity;
}

void RigidBody::setAngularVelocity(const glm::vec3 & velocity)
{
    if (m_static)
    {
        return;
    }

    m_angularVelocity = velocity;
}

void RigidBody::setForce(const glm::vec3 & force)
{
    if (m_static)
    {
        return;
    }

    m_force = force;
}

void RigidBody::setStatic(bool isStatic)
{
    m_static = isStatic;
}

void RigidBody::setIndex(size_t index)
{
    m_index = index;
}

void RigidBody::applyForce(const glm::vec3 & force)
{
    if (m_static)
    {
        return;
    }

    m_force += force;
}

void RigidBody::predictTransform(float seconds, Transform3D & prediction)
{
    prediction.setCenter(transform().center());
    prediction.setScale(transform().scale());
    prediction.setPosition(transform().position() + seconds * m_linearVelocity);
    prediction.setOrientation(transform().orientation() * glm::quat(seconds * m_angularVelocity));
}

void RigidBody::updateWorldInertia()
{
    m_worldInvInertia = transform().basis() * m_localInvInertia * glm::transpose(transform().basis());
}

void RigidBody::integrateVelocities(float seconds)
{
    if (m_static)
    {
        return;
    }

    m_linearVelocity += m_inverseMass * m_force * seconds;
    m_angularVelocity += m_worldInvInertia * m_torque * seconds;
}

std::string RigidBody::toString() const
{
    std::stringstream stream;
    stream << "{" << transform().position() << ", " << transform().orientation() << "}";
    return stream.str();
}

}

