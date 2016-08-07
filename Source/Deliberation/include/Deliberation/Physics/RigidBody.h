#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/CollisionObject.h>

namespace deliberation
{

class DELIBERATION_API RigidBody:
    public CollisionObject
{
public:
    RigidBody(const std::shared_ptr<CollisionShape> & shape,
              const Transform3D & transform = Transform3D(),
              float inverseMass = 1.0f / 1.0f,
              const glm::vec3 & linearVelocity = {},
              const glm::vec3 & angularVelocity = {});

    float mass() const;
    float inverseMass() const;
    const glm::vec3 & linearVelocity() const;
    const glm::vec3 & angularVelocity() const;
    const glm::vec3 & force() const;
    bool isStatic() const;

    void setInverseMass(float mass);
    void setLinearVelocity(const glm::vec3 & velocity);
    void setAngularVelocity(const glm::vec3 & velocity);
    void setForce(const glm::vec3 & force);
    void applyForce(const glm::vec3 & force);
    void setStatic(bool isStatic);

    void integrateVelocities(float seconds);

    void predictTransform(float seconds, Transform3D & prediction);

    std::string toString() const;

private:
    float                           m_inverseMass = 1.0f / 1.0f;
    glm::vec3                       m_linearVelocity;
    glm::vec3                       m_angularVelocity;
    glm::vec3                       m_force;
    bool                            m_static = false;
};

}

