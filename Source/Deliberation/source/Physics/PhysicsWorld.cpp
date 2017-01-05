#include <Deliberation/Physics/PhysicsWorld.h>

#include <iostream>
#include <sstream>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/Contacts/Contact.h>
#include <Deliberation/Physics/ReferenceBroadphase.h>
#include <Deliberation/Physics/Narrowphase.h>
#include <Deliberation/Physics/RigidBody.h>

namespace
{
    const auto BAUMGARTE = 0.005f;
    const auto LINEAR_SLOP = 0.005f;
    const auto MAX_LINEAR_CORRECTION = 0.2f;
}

namespace deliberation
{

PhysicsWorld::PhysicsWorld(float timestep):
    m_timestep(timestep)
{
    m_narrowphase.reset(new Narrowphase());
    m_broadphase.reset(new ReferenceBroadphase(*m_narrowphase));
}

PhysicsWorld::~PhysicsWorld() = default;

const PhysicsWorld::RigidBodies & PhysicsWorld::rigidBodies() const
{
    return m_rigidBodies;
}

float PhysicsWorld::timestep() const
{
    return m_timestep;
}

Narrowphase & PhysicsWorld::narrowphase()
{
    return *m_narrowphase;
}

const Narrowphase & PhysicsWorld::narrowphase() const
{
    return *m_narrowphase;
}

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
    m_timeAccumulator += seconds;
    while (m_timeAccumulator >= m_timestep)
    {
        performTimestep(m_timestep);
        m_timeAccumulator -= m_timestep;
    }
}

void PhysicsWorld::performTimestep(float seconds)
{
    for (auto & body : m_rigidBodies)
    {
        body->applyForce({0.0f, -m_gravity * body->mass(), 0.0f});
        body->integrateVelocities(seconds);
        body->updateWorldInertia();
    }

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

    /**
     * Clear forces
     */
    for (auto & body : m_rigidBodies)
    {
        body->setForce({});
    }
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

    /**
     * Warm starting
     */
    for (auto & contact : m_narrowphase->contacts())
    {
        warmStart(*contact);
    }

    /**
     * Iterative solving
     */

//    std::cout << "Solve velocities:" << std::endl;
    for (auto i = 0; i < m_numVelocityIterations; i++)
    {
        for (auto & contact : m_narrowphase->contacts())
        {
            solveContactVelocities(*contact);
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

void PhysicsWorld::warmStart(Contact & contact) const
{
    if (!contact.intersect())
    {
        return;
    }

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto mA = bodyA.inverseMass();
    auto & iA = bodyA.worldInverseInertia();
    auto & vA = bodyA.linearVelocity();
    auto & wA = bodyA.angularVelocity();
    auto & cA = bodyA.transform().position();

    auto mB = bodyB.inverseMass();
    auto & iB = bodyB.worldInverseInertia();
    auto & vB = bodyB.linearVelocity();
    auto & wB = bodyB.angularVelocity();
    auto & cB = bodyB.transform().position();

    for (uint p = 0; p < contact.numPoints(); p++)
    {
        auto & point = contact.point(p);

        auto & n = point.normal;

        auto rA = point.position - cA;
        auto rB = point.position - cB;

        auto lambda = point.normalImpulseAccumulator;

        auto J = lambda * n;

        bodyA.setLinearVelocity(vA - mA * J);
        bodyA.setAngularVelocity(wA - iA * glm::cross(rA, J));

        bodyB.setLinearVelocity(vB + mB * J);
        bodyB.setAngularVelocity(wB + iB * glm::cross(rB, J));
    }
}

void PhysicsWorld::solveContactVelocities(Contact & contact)
{
    if (!contact.intersect())
    {
        return;
    }

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto mA = bodyA.inverseMass();
    auto & iA = bodyA.worldInverseInertia();
    auto & vA = bodyA.linearVelocity();
    auto & wA = bodyA.angularVelocity();
    auto & cA = bodyA.transform().position();

    auto mB = bodyB.inverseMass();
    auto & iB = bodyB.worldInverseInertia();
    auto & vB = bodyB.linearVelocity();
    auto & wB = bodyB.angularVelocity();
    auto & cB = bodyB.transform().position();

    // Coefficient of restitution
    auto e = contact.restitution();

    /**
     * Solve tangent velocities
     */
    for (uint p = 0; p < contact.numPoints(); p++)
    {
        auto & point = contact.point(p);

        auto rA = point.position - cA;
        auto rB = point.position - cB;

        // Relative velocity along normal
        auto vra = bodyA.localVelocity(rA);
        auto vrb = bodyB.localVelocity(rB);
        auto vRel = vrb - vra;
        auto vRelNormal = glm::dot(point.normal, vRel);

        // Relative velocity along tangent
        auto vRelTangent = vRel - (vRelNormal * point.normal);

        if (vRelTangent == glm::vec3(0.0f)) {
            continue;
        }

        auto tangent = glm::normalize(vRelTangent);

        auto tangentMass = glm::dot(glm::cross(iA * glm::cross(rA, tangent), rA) +
                                    glm::cross(iB * glm::cross(rB, tangent), rB), tangent) + mA + mB;

        if (tangentMass == 0.0f) {
            continue;
        }

        auto lambda = -glm::length(vRelTangent) / tangentMass;

        auto maxFriction = contact.friction() * point.normalImpulseAccumulator;
        auto newImpulse = std::max(-maxFriction, std::min<float>(maxFriction, point.tangentImpulseAccumulator + lambda));
        lambda = newImpulse - point.tangentImpulseAccumulator;

        auto P = lambda * tangent;

        bodyA.setLinearVelocity(vA - mA * P);
        bodyA.setAngularVelocity(wA - iA * glm::cross(rA, P));

        bodyB.setLinearVelocity(vB + mB * P);
        bodyB.setAngularVelocity(wB + iB * glm::cross(rB, P));
    }

    /**
     * Solve normal velocities
     */
    for (uint p = 0; p < contact.numPoints(); p++)
    {
        auto & point = contact.point(p);

        auto rA = point.position - cA;
        auto rB = point.position - cB;

        auto & n = point.normal;
        auto normalMass = point.normalMass;
        auto velocityBias = point.velocityBias;

        // Relative velocity along normal
        auto vra = bodyA.localVelocity(rA);
        auto vrb = bodyB.localVelocity(rB);
        auto vRel = glm::dot(n, vrb - vra);

        //
        auto vDelta = velocityBias - vRel;

        auto lambda = vDelta / normalMass;

        auto newNormalImpulseAccumulator = std::max(lambda + point.normalImpulseAccumulator, 0.0f);
        lambda = newNormalImpulseAccumulator - point.normalImpulseAccumulator;
        point.normalImpulseAccumulator += lambda;

        // Bias lambda
        auto bias = (BAUMGARTE * std::max(point.depth - LINEAR_SLOP, 0.0f)) / m_timestep;

        // J - impulse magnitude
        auto J = (lambda + bias) * n;

        bodyA.setLinearVelocity(vA - mA * J);
        bodyA.setAngularVelocity(wA - iA * glm::cross(rA, J));

        bodyB.setLinearVelocity(vB + mB * J);
        bodyB.setAngularVelocity(wB + iB * glm::cross(rB, J));
    }
}

void PhysicsWorld::solvePositions(Contact & contact)
{
    if (!contact.intersect())
    {
        return;
    }

    contact.update();

    auto & bodyA = contact.bodyA();
    auto & bodyB = contact.bodyB();

    auto & transformA = bodyA.transform();
    auto & transformB = bodyB.transform();

    auto mA = bodyA.inverseMass();
    auto mB = bodyB.inverseMass();

    auto & iA = bodyA.worldInverseInertia();
    auto & iB = bodyB.worldInverseInertia();

    auto & cA = transformA.position();
    auto & cB = transformB.position();

    for (uint p = 0; p < contact.numPoints(); p++)
    {
        auto & point = contact.point(p);

        auto & n = point.normal;

        auto position = point.position;
        auto separation = -point.depth;

        auto rA = position - cA;
        auto rB = position - cB;

        // Track max constraint error.
        //  auto minSeparation = std::min(minSeparation, separation);

        // Prevent large corrections and allow slop.
        auto C = std::min(std::max(0.5f * (separation + LINEAR_SLOP), -MAX_LINEAR_CORRECTION), 0.0f);

        // Compute the effective mass.
        auto normalMass = point.normalMass;

        // Compute normal impulse
        auto impulse = normalMass > 0.0f ? -C / normalMass : 0.0f;

        auto P = impulse * n;

        std::cout << P << " " << separation << " " << (-mA * P) << " " << (mB * P) << std::endl;

        transformA.setPosition(transformA.position() - mA * P);
        transformA.setOrientation(QuaternionAxisRotation(transformA.orientation(), iA * glm::cross(rA, -P)));

        transformB.setPosition(transformB.position() + mB * P);
        transformB.setOrientation(QuaternionAxisRotation(transformB.orientation(), iB * glm::cross(rB, P)));
    }
}

}

