#include <Deliberation/Physics/RigidBody.h>

#include <sstream>

#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

RigidBody::RigidBody(const std::shared_ptr<CollisionShape> & shape,
                     const Transform3D & transform,
                     float mass,
                     const glm::vec3 & linearVelocity,
                     const glm::vec3 & angularVelocity):
    CollisionObject(shape, transform),
    m_inverseMass(0.0f),
    m_linearVelocity(linearVelocity),
    m_angularVelocity(angularVelocity)
{
    setMass(mass);
}

float RigidBody::inverseMass() const
{
    if (m_static)
    {
        return 0.0f;
    }

    return m_inverseMass;
}

float RigidBody::mass() const
{
    if (m_static)
    {
        return 0.0f;
    }

    return m_inverseMass != 0.0f ? 1.0f / m_inverseMass : 0.0f;
}

float RigidBody::restitution() const
{
    return m_restitution;
}

float RigidBody::friction() const
{
    return m_friction;
}

const glm::mat3 & RigidBody::worldInverseInertia() const
{
    static glm::mat3 staticBodyInertia(0.0f);

    if (m_static)
    {
        return staticBodyInertia;
    }

    return m_worldInverseInertia;
}

const glm::vec3 & RigidBody::linearVelocity() const
{
    static glm::vec3 staticBodyLinearVelocity(0.0f);
    if (m_static)
    {
        return staticBodyLinearVelocity;
    }

    return m_linearVelocity;
}

const glm::vec3 & RigidBody::angularVelocity() const
{
    static glm::vec3 staticAngularVelocity(0.0f);
    if (m_static)
    {
        return staticAngularVelocity;
    }

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

std::shared_ptr<RigidBodyPayload> & RigidBody::payload()
{
    return m_payload;
}

const std::shared_ptr<RigidBodyPayload> & RigidBody::payload() const
{
    return m_payload;
}

bool RigidBody::isStatic() const
{
    return m_static;
}

glm::vec3 RigidBody::localVelocity(const glm::vec3 & r) const
{
    if (m_static)
    {
        return glm::vec3(0.0f);
    }

    return m_linearVelocity + glm::cross(m_angularVelocity, r);
}

void RigidBody::setMass(float mass)
{
    setInverseMass(mass > 0.0f ? 1.0f / mass : 0.0f);
}

void RigidBody::setInverseMass(float inverseMass)
{
    m_inverseMass = inverseMass;
    auto localInertia = shape()->localInertia() * mass();
    m_localInverseInertia = glm::inverse(localInertia);
}

void RigidBody::setRestitution(float restitution)
{
    m_restitution = restitution;
}

void RigidBody::setFriction(float friction)
{
    m_friction = friction;
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

void RigidBody::setPayload(const std::shared_ptr<RigidBodyPayload> & payload)
{
    m_payload = payload;
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

    auto w = m_angularVelocity * seconds;

    auto angle = glm::length(w);

    if (angle > 0.0001f)
    {
        auto axis = glm::normalize(w);

        auto quat = glm::rotate(glm::quat(), angle, axis);

        prediction.setOrientation(quat * transform().orientation());
    }
    else
    {
        prediction.setOrientation(transform().orientation());
    }

}

void RigidBody::updateWorldInertia()
{
    m_worldInverseInertia = transform().basis() * m_localInverseInertia * glm::transpose(transform().basis());
}

void RigidBody::integrateVelocities(float seconds)
{
    if (m_static)
    {
        return;
    }

    m_linearVelocity += m_inverseMass * m_force * seconds;
    m_angularVelocity += m_worldInverseInertia * m_torque * seconds;
}

std::string RigidBody::toString() const
{
    std::stringstream stream;
    stream << "{" << transform().position() << ", " << transform().orientation() << "}";
    return stream.str();
}

}

