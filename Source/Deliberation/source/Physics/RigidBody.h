#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/ECS/Entity.h>

#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/CollisionObject.h>



namespace deliberation
{

struct RigidBodyPayload
{
    virtual ~RigidBodyPayload() = default;
};

class RigidBody final:
    public std::enable_shared_from_this<RigidBody>,
    public CollisionObject
{
public:
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

public:
    RigidBody(const std::shared_ptr<CollisionShape> & shape,
              const Transform3D & transform = Transform3D(),
              const glm::vec3 & linearVelocity = {},
              const glm::vec3 & angularVelocity = {});

    float inverseMass() const;
    float restitution() const;
    float friction() const;
    const glm::mat3 & worldInverseInertia() const;
    const glm::vec3 & linearVelocity() const;
    const glm::vec3 & angularVelocity() const;
    const glm::vec3 & force() const;
    bool isStatic() const;
    size_t index() const;
    Entity & entity() const;

    glm::vec3 localVelocity(const glm::vec3 & r) const;

    void setRestitution(float restitution);
    void setFriction(float friction);
    void setLinearVelocity(const glm::vec3 & velocity);
    void setAngularVelocity(const glm::vec3 & velocity);
    void setForce(const glm::vec3 & force);
    void setStatic(bool isStatic);
    void setIndex(size_t index);
    void setEntity(Entity entity);

    void applyForce(const glm::vec3 & force);
    void applyImpulse(const glm::vec3 & point, const glm::vec3 & impulse);

    void integrateVelocities(float seconds);
    void adjustCenterOfMass();

    void predictTransform(float seconds, Transform3D & prediction);

    std::string toString() const;

private:
    float               m_restitution = 0.1f;
    float               m_friction    = 0.03f;

    mutable glm::mat3   m_worldInverseInertia;

    glm::vec3           m_linearVelocity;
    glm::vec3           m_angularVelocity;

    glm::vec3           m_force;
    glm::vec3           m_torque;

    bool                m_static = false;
    size_t              m_index = INVALID_INDEX;

    mutable Entity      m_entity;
};

}

