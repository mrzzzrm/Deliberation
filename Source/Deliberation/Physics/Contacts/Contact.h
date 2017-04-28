#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Span.h>



namespace deliberation
{

constexpr uint MAX_NUM_CONTACT_POINTS = 4;

class Contact;
class RigidBody;

struct Intersection
{
    glm::vec3 position;
    glm::vec3 normal;
    float     depth = 0.0f;
};

struct ContactPoint:
    public Intersection
{
    glm::vec3 relativePositionA;
    glm::vec3 relativePositionB;
    float     velocityBias = 0.0f;
    float     normalMass;
    float     tangentMass;
    float     normalImpulseAccumulator = 0.0f;
    float     tangentImpulseAccumulator = 0.0f;

    void initialize(const Contact & contact, const Intersection & intersection);
    void update(const Contact & contact, const Intersection & intersection);
};

class Contact
{
public:
    Contact(RigidBody & bodyA, RigidBody & bodyB);
    virtual ~Contact();

    RigidBody & bodyA() const;
    RigidBody & bodyB() const;

    bool intersect() const;

    float restitution() const;
    float friction() const;

    uint numPoints() const;
    ContactPoint & point(uint index);

    void updatePoints(const Span<Intersection> & intersections);
    void clearPoints();

    virtual void update() = 0;

private:
    bool intersectionMatchesPoint(const Intersection & intersection, const ContactPoint & point) const;

protected:
    RigidBody &  m_bodyA;
    RigidBody &  m_bodyB;

    bool         m_intersect;

    float        m_resitution;
    float        m_friction;

    uint         m_numPoints;
    ContactPoint m_points[MAX_NUM_CONTACT_POINTS];
};

}