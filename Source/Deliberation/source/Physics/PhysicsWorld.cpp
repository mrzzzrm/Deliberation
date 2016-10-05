#include <Deliberation/Physics/PhysicsWorld.h>

#include <iostream>
#include <sstream>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/Contacts/Contact.h>
#include <Deliberation/Physics/ReferenceBroadphase.h>
#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

PhysicsWorld::PhysicsWorld(float timestep):
    m_timestep(timestep)
{
    m_narrowphase.reset(new Narrowphase());
    m_broadphase.reset(new ReferenceBroadphase(*m_narrowphase));
}

PhysicsWorld::~PhysicsWorld() = default;

void PhysicsWorld::addRigidBody(const std::shared_ptr<RigidBody> & body)
{
    auto index = m_rigidBodies.insert(body);
    body->setIndex(index);

    auto proxy = m_broadphase->createProxy(*body);
    body->setProxy(proxy);
}

void PhysicsWorld::removeRigidBody(const RigidBody & body)
{
    auto index = body.index();
    m_rigidBodies.erase(index);
    m_broadphase->removeProxy(*body.proxy());
}

void PhysicsWorld::setGravity(float gravity)
{
    m_gravity = gravity;
}

void PhysicsWorld::update(float seconds)
{
    for (auto & body : m_rigidBodies)
    {
        if (body->isStatic())
        {
            continue;
        }

        body->applyForce({0.0f, -m_gravity * body->mass(), 0.0f});
        body->integrateVelocities(seconds);
        body->updateWorldInertia();
    }

    auto time = 0.0f;
    for (; time < seconds; time += m_timestep)
    {
        performTimestep(m_timestep);
    }

    auto remainder = seconds - (time - m_timestep);
    if (remainder > 0)
    {
        performTimestep(remainder);
    }

    /**
     * Clear forces
     */
    for (auto & body : m_rigidBodies)
    {
        body->setForce({});
    }
}

void PhysicsWorld::performTimestep(float seconds)
{
    /**
     * Update broadphase
     */
    for (auto & body : m_rigidBodies)
    {
        m_broadphase->setProxyBounds(*body->proxy(), body->bounds());
    }

    m_broadphase->checkProximities();
    m_narrowphase->updateContacts();

    solve();

    integrateTransforms(seconds);
}

void PhysicsWorld::integrateTransforms(float seconds)
{
    for (auto & body : m_rigidBodies)
    {
        if (body->isStatic())
        {
            continue;
        }

        Transform3D predictedTransform;
        body->predictTransform(seconds, predictedTransform);
        body->setTransform(predictedTransform);
    }
}

std::string PhysicsWorld::toString() const
{
    std::stringstream stream;
    stream << "{\n";
    for (auto & body : m_rigidBodies)
    {
        stream << "  " << body->toString() << "\n";
    }
    stream << "}\n";

    return stream.str();
}

void PhysicsWorld::solve()
{
    if (m_narrowphase->contacts().empty())
    {
        return;
    }

    std::cout << "Solve velocities:" << std::endl;
    for (auto i = 0; i < m_numVelocityIterations; i++)
    {
        for (auto & contact : m_narrowphase->contacts())
        {
            solveVelocities(*contact);
        }
    }

    std::cout << "Solve positions:" << std::endl;
    for (auto i = 0; i < m_numPositionIterations; i++)
    {
        for (auto & contact : m_narrowphase->contacts())
        {
            solvePositions(*contact);
        }
    }
}

void PhysicsWorld::solveVelocities(Contact & contact)
{
    if (!contact.intersect())
    {
        return;
    }

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto mA = bodyA.inverseMass();
    auto & iA = bodyA.worldInvInertia();
    auto & vA = bodyA.linearVelocity();
    auto & wA = bodyA.angularVelocity();
    auto & pA = bodyA.transform().position();
    auto cA = contact.position() - pA;

    auto mB = bodyB.inverseMass();
    auto & iB = bodyB.worldInvInertia();
    auto & vB = bodyB.linearVelocity();
    auto & wB = bodyB.angularVelocity();
    auto & pB = bodyB.transform().position();
    auto cB = contact.position() - pB;

    auto & normal = contact.normal();
    auto normalMass = contact.normalMass();
    auto velocityBias = contact.velocityBias();
    auto normalImpulse = contact.normalImpulse();

    // Relative velocity at contact
    auto aA = glm::cross(wA, cA);
    auto aB = glm::cross(wB, cB);

    auto relativeVelocityAtContact = vB + aB - vA - aA;

    auto relativeVelocityAtNormal = glm::dot(relativeVelocityAtContact, normal);
    auto lambda = -normalMass * (relativeVelocityAtNormal - velocityBias);

    // Clamp the accumulated impulse
    auto newImpulse = std::max(normalImpulse + lambda, 0.0f);
    lambda = newImpulse - normalImpulse;

    contact.setNormalImpulse(newImpulse);

    // Apply contact impulse
    auto contactImpulse = lambda * normal;

    std::cout << "  " << &contact << "(" << mA << ": " << contactImpulse << " " << normalMass << " " << " "  << velocityBias << " " << normal << std::endl;
    std::cout << "    rV: " << relativeVelocityAtContact << std::endl;
    std::cout << "    rVn: " << relativeVelocityAtNormal << std::endl;
    std::cout << "    Impulse: " << normalImpulse << " -> " << newImpulse << std::endl;
    std::cout << "    " << vA << " " << wA << std::endl;
    std::cout << "    " << vB << " " << wB << std::endl;

    bodyA.setLinearVelocity(vA - mA * contactImpulse);
    bodyA.setAngularVelocity(wA - iA * glm::cross(cA, contactImpulse));

    bodyB.setLinearVelocity(vB + mB * contactImpulse);
    bodyB.setAngularVelocity(wB + iB * glm::cross(cB, contactImpulse));
}

void PhysicsWorld::solvePositions(Contact & contact)
{
    if (!contact.intersect())
    {
        return;
    }

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto & transformA = bodyA.transform();
    auto & transformB = bodyB.transform();

    auto mA = bodyA.inverseMass();
    auto mB = bodyB.inverseMass();

    auto & iA = bodyA.worldInvInertia();
    auto & iB = bodyB.worldInvInertia();

    auto & cA = transformA.position();

    auto cB = transformB.position();

    auto normal = contact.normal();

//    b2Vec2 point = psm.point;
    auto separation = contact.separation();

//    auto rA = point - cA;
//    auto rB = point - cB;

    // Track max constraint error.
  //  auto minSeparation = std::min(minSeparation, separation);

    auto baumgarte = 0.2f;
    auto linearSlop = 0.0005f;
    auto maxLinearCorrection = 0.2f;

    // Prevent large corrections and allow slop.
    auto C = std::min(std::max(baumgarte * (separation + linearSlop), -maxLinearCorrection), 0.0f);

    // Compute the effective mass.
//    auto rnA = glm::cross(rA, normal);
//    auto rnB = glm::cross(rB, normal);
    auto K = mA + mB/* + iA * rnA * rnA + iB * rnB * rnB*/;

    // Compute normal impulse
    auto impulse = K > 0.0f ? - C / K : 0.0f;

    auto P = impulse * normal;

    transformA.setPosition(transformA.position() - mA * P);
//    transformA.setOrientation(transformA.orientation() - iA * glm::cross(cA, P))
//    aA -= iA * b2Cross(rA, P);
//
    transformB.setPosition(transformB.position() + mB * P);
//    aB += iB * b2Cross(rB, P);
}

}

