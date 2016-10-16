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

    {
        auto timeStepped = 0.0f;
        for(;;)
        {
            auto timeStep = std::min    (seconds - timeStepped, m_timestep);
            performTimestep(timeStep);
            timeStepped += timeStep;

            if (timeStep < m_timestep)
            {
                break;
            }
        }
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

//    std::cout << "Solve velocities:" << std::endl;
    for (auto i = 0; i < m_numVelocityIterations; i++)
    {
        for (auto & contact : m_narrowphase->contacts())
        {
            solveContactVelocities(*contact);
        }
    }

//    std::cout << "Solve positions:" << std::endl;
//    for (auto i = 0; i < m_numPositionIterations; i++)
//    {
//        for (auto & contact : m_narrowphase->contacts())
//        {
//            solvePositions(*contact);
//        }
//    }
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

    for (uint p = 0; p < contact.numPoints(); p++)
    {
        auto & point = contact.point(p);

        auto rA = point.position - cA;
        auto rB = point.position - cB;

        auto & n = point.normal;
        auto normalMass = point.normalMass;
        auto velocityBias = point.velocityBias;

        // Relative velocity along normal
        auto vra = vA + glm::cross(wA, rA);
        auto vrb = vB + glm::cross(wB, rB);
        auto vRel = glm::dot(n, vrb - vra);

        //
        auto vDelta = velocityBias - vRel;

        auto lambda = vDelta / normalMass;

        auto newNormalImpulseAccumulator = std::max(lambda + point.normalImpulseAccumulator, 0.0f);
        lambda = newNormalImpulseAccumulator - point.normalImpulseAccumulator;
        point.normalImpulseAccumulator += lambda;

        std::cout << "Impulse: " << lambda << " " << n << " " << point.normalImpulseAccumulator << " " << normalMass           << std::endl;

        // J - impulse magnitude
        auto J = lambda * n;

        bodyA.setLinearVelocity(vA - mA * J);
        bodyA.setAngularVelocity(wA - iA * glm::cross(rA, J));

        bodyB.setLinearVelocity(vB + mB * J);
        bodyB.setAngularVelocity(wB + iB * glm::cross(rB, J));
    }
}

//void PhysicsWorld::solvePositions(Contact & contact)
//{
//    if (!contact.intersect())
//    {
//        return;
//    }
//
//    auto & bodyA = contact.bodyA();
//    auto & bodyB = contact.bodyB();
//
//    auto & transformA = bodyA.transform();
//    auto & transformB = bodyB.transform();
//
//    auto mA = bodyA.inverseMass();
//    auto mB = bodyB.inverseMass();
//
//    auto & iA = bodyA.worldInverseInertia();
//    auto & iB = bodyB.worldInverseInertia();
//
//    auto & cA = transformA.position();
//    auto & cB = transformB.position();
//
//    auto n = contact.normal();
//
//    auto point = contact.position();
//    auto separation = -contact.separation();
//
//    auto rA = point - cA;
//    auto rB = point - cB;
//
//
//    // Track max constraint error.
//  //  auto minSeparation = std::min(minSeparation, separation);
//
//    auto baumgarte = 0.2f;
//    auto linearSlop = 0.0005f;
//    auto maxLinearCorrection = 0.2f;
//
//    // Prevent large corrections and allow slop.
//    auto C = std::min(std::max(baumgarte * (separation + linearSlop), -maxLinearCorrection), 0.0f);
//
//    // Compute the effective mass.
//    // Denominator
//    auto tA = glm::cross(rA, n);
//    auto tB = glm::cross(rB, n);
//    auto itA = iA * tA;
//    auto itB = iB * tB;
//    auto wiA = glm::cross(itA, rA);
//    auto wiB = glm::cross(itB, rB);
//    auto dwvA = glm::dot(n, wiA);
//    auto dwvB = glm::dot(n, wiB);
//    auto K = mA + mB + glm::dot(wiA + wiB, n);
//
//
//    // Compute normal impulse
//    auto impulse = K > 0.0f ? - C / K : 0.0f;
//
//    auto P = impulse * n;
//
//    std::cout << "Separation: " << separation << " " << C << " " << K << " = " << P << std::endl;
//
//    transformA.setPosition(transformA.position() - mA * P);
//    transformA.setOrientation(transformA.orientation() * glm::quat(iA * glm::cross(cA, -P)));
////    aA -= iA * b2Cross(rA, P);
////
//    transformB.setPosition(transformB.position() + mB * P);
//    transformB.setOrientation(transformB.orientation() * glm::quat(iB * glm::cross(cB, P)));
////    aB += iB * b2Cross(rB, P);
//}

}

