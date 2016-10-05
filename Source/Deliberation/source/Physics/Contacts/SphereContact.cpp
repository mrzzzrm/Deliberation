#include <Deliberation/Physics/Contacts/SphereContact.h>

#include <iostream>

#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/SphereShape.h>

namespace deliberation
{

SphereContact::SphereContact(RigidBody & bodyA, RigidBody & bodyB):
    Contact(bodyA, bodyB),
    m_shapeA(*(SphereShape*)bodyA.shape().get()),
    m_shapeB(*(SphereShape*)bodyB.shape().get())
{

}

void SphereContact::update()
{
    auto pA = m_bodyA.transform().position();
    auto pB = m_bodyB.transform().position();

    auto delta = pA - pB;
    auto distance = glm::dot(delta, delta);

    auto threshold = m_shapeA.radius() + m_shapeB.radius();
    threshold = threshold * threshold;

    m_intersect = distance < threshold;

    if (m_intersect)
    {
        m_normal = glm::normalize(pB - pA);
        m_localPointA = pA + m_normal * m_shapeA.radius();
        m_localPointB = pB - m_normal * m_shapeB.radius();
        m_position = 0.5f * (m_localPointA + m_localPointB);
        m_separation = glm::dot(pB - pA, m_normal) - m_shapeA.radius() - m_shapeB.radius();
    }

    auto & vA = m_bodyA.linearVelocity();
    auto & vB = m_bodyB.linearVelocity();

    auto mA = m_bodyA.inverseMass();
    auto mB = m_bodyB.inverseMass();

   // vcp->rA = worldManifold.points[j] - cA;
   // vcp->rB = worldManifold.points[j] - cB;

  //  float32 rnA = b2Cross(vcp->rA, vc->normal);
 //   float32 rnB = b2Cross(vcp->rB, vc->normal);

    auto kNormal = mA + mB /* + iA * rnA * rnA + iB * rnB * rnB*/;

    m_normalMass = kNormal > 0.0f ? 1.0f / kNormal : 0.0f;

 //   b2Vec2 tangent = b2Cross(vc->normal, 1.0f);

   // float32 rtA = b2Cross(vcp->rA, tangent);
   // float32 rtB = b2Cross(vcp->rB, tangent);

 //   float32 kTangent = mA + mB + iA * rtA * rtA + iB * rtB * rtB;

    //vcp->tangentMass = kTangent > 0.0f ? 1.0f /  kTangent : 0.0f;

    // Setup a velocity bias for restitution.
    m_velocityBias = 0.0f;
    auto vRel = glm::dot(m_normal, vB - vA /*- b2Cross(wA, vcp->rA)+ glm::cross(wB, vcp->rB) */);
    if (vRel < -1.0f)
    {
        auto restitution = std::max(m_bodyA.restitution(), m_bodyB.restitution());
        m_velocityBias = -restitution * vRel;
    }
}

}