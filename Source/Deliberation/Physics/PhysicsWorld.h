#pragma once

#include <memory>

#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Physics/PrimitiveTestAlgorithm.h>
#include <Deliberation/Physics/PrimitiveTester.h>

class btCollisionConfiguration;
class btCollisionDispatcher;
class btConstraintSolver;
class btBroadphaseInterface;

namespace deliberation
{
class RigidBody;

class PhysicsWorld final
{
public:
    using RigidBodies = SparseVector<std::shared_ptr<RigidBody>>;

public:
    PhysicsWorld(float timestep = 1.0f / 60.0f);

    /**
     * Returns the amount of time that is going to be simulated if @param seconds would be
     * passed to PhysicsWorld::update()
     */
    float probeNextSimulationStepSeconds(float seconds) const;

    PrimitiveTester & primitiveTester() { return m_primitiveTester; }
    const PrimitiveTester & primitiveTester() const { return m_primitiveTester; }
    const RigidBodies & rigidBodies() const { return m_rigidBodies; }
    float               timestep() const { return m_timestep; }
    const std::shared_ptr<btDiscreteDynamicsWorld> & bulletDynamicsWorld() const { return m_dynamicsWorld; }

    void setGravity(const glm::vec3 & gravity);

    void addRigidBody(const std::shared_ptr<RigidBody> & body);
    void removeRigidBody(const std::shared_ptr<RigidBody> & body);

    float update(float seconds);

    void lineCast(
        const Ray3D &                                            ray,
        const std::function<bool(const RayCastIntersection &)> & handler) const;

private:
    float                                       m_timestep;
    float                                       m_timeAccumulator = 0.0f;
    RigidBodies                                 m_rigidBodies;
    PrimitiveTester                             m_primitiveTester;

    u64                                         m_numSimulatedSteps = 0;

    std::shared_ptr<btCollisionConfiguration>   m_collisionConfiguration;
    std::shared_ptr<btCollisionDispatcher>      m_collisionDispatcher;
    std::shared_ptr<btConstraintSolver>         m_constraintSolver;
    std::shared_ptr<btBroadphaseInterface>      m_broadphase;
    std::shared_ptr<btDiscreteDynamicsWorld>    m_dynamicsWorld;
};
}
