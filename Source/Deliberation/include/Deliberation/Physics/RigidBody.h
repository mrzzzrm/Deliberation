#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/CollisionObject.h>

namespace deliberation
{

struct RigidBodyPayload
{
    virtual ~RigidBodyPayload() = default;
};

class DELIBERATION_API RigidBody final:
    public CollisionObject
{
public:
    RigidBody(const std::shared_ptr<CollisionShape> & shape,
              const Transform3D & transform = Transform3D(),
              float mass = 1.0f,
              const glm::vec3 & linearVelocity = {},
              const glm::vec3 & angularVelocity = {});

    float mass() const;
    float inverseMass() const;
    float restitution() const;
    float friction() const;
    const glm::mat3 & worldInverseInertia() const;
    const glm::vec3 & linearVelocity() const;
    const glm::vec3 & angularVelocity() const;
    const glm::vec3 & force() const;
    bool isStatic() const;
    size_t index() const;
    std::unique_ptr<RigidBodyPayload> & payload();
    const std::unique_ptr<RigidBodyPayload> & payload() const;

    glm::vec3 localVelocity(const glm::vec3 & r) const;

    void setMass(float mass);
    void setInverseMass(float mass);
    void setRestitution(float restitution);
    void setFriction(float friction);
    void setLinearVelocity(const glm::vec3 & velocity);
    void setAngularVelocity(const glm::vec3 & velocity);
    void setForce(const glm::vec3 & force);
    void setStatic(bool isStatic);
    void setIndex(size_t index);
    void setPayload(std::unique_ptr<RigidBodyPayload> && payload);

    void applyForce(const glm::vec3 & force);

    void integrateVelocities(float seconds);

    void predictTransform(float seconds, Transform3D & prediction);

    void updateWorldInertia();

    std::string toString() const;

private:
    float       m_inverseMass = 1.0f / 1.0f;
    float       m_restitution = 0.1f;
    float       m_friction    = 0.03f;

    glm::mat3   m_localInverseInertia;
    glm::mat3   m_worldInverseInertia;

    glm::vec3   m_linearVelocity;
    glm::vec3   m_angularVelocity;

    glm::vec3   m_force;
    glm::vec3   m_torque;

    bool        m_static = false;
    size_t      m_index = -1;

    std::unique_ptr<RigidBodyPayload>
                m_payload;
};

}

