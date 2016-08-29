#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class RigidBody;

class DELIBERATION_API Contact
{
public:
    Contact(RigidBody & bodyA, RigidBody & bodyB);
    virtual ~Contact();

    RigidBody & bodyA() const;
    RigidBody & bodyB() const;
    bool intersect() const;
    const glm::vec3 & localPointA() const;
    const glm::vec3 & localPointB() const;
    const glm::vec3 & normal() const;
    float normalMass() const;
    float velocityBias() const;
    float normalImpulse() const;;
    float separation() const;

    void setNormalImpulse(float normalImpulse);

    virtual void update() = 0;

protected:
    RigidBody & m_bodyA;
    RigidBody & m_bodyB;
    bool        m_intersect;
    glm::vec3   m_localPointA;
    glm::vec3   m_localPointB;
    glm::vec3   m_normal;
    float       m_normalMass = 0.0f;
    float       m_velocityBias = 0.0f;
    float       m_normalImpulse = 0.0f;
    float       m_separation;
};

}