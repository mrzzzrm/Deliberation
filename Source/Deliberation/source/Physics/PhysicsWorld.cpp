#include <Deliberation/Physics/PhysicsWorld.h>

#include <iostream>
#include <sstream>

#include <Deliberation/Core/Math/Transform3D.h>

#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{

PhysicsWorld::PhysicsWorld(float timestep):
    m_timestep(timestep)
{

}

void PhysicsWorld::addRigidBody(std::shared_ptr<RigidBody> & body)
{
    m_rigidBodies.insert(body);
}

void PhysicsWorld::removeRigidBody(std::shared_ptr<RigidBody> & body)
{

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
    std::cout << "PhysicsWorld::performTimestep(): " << seconds << std::endl;



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

}

