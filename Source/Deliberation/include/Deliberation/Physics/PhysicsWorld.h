#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/SparseVector.h>

#include <Deliberation/Physics/NarrowphasePrimitiveTest.h>

namespace deliberation
{

class Broadphase;
class Contact;
class Context;
class Narrowphase;
class RigidBody;

class DELIBERATION_API PhysicsWorld final
{
public:
    using RigidBodies = SparseVector<std::shared_ptr<RigidBody>>;

public:
    PhysicsWorld(float timestep = 1.0f / 60.0f);
    ~PhysicsWorld();

    const RigidBodies & rigidBodies() const;
    float timestep() const;
    Narrowphase & narrowphase();
    const Narrowphase & narrowphase() const;

    /**
     * If @seconds would be handed to update(), returns how many seconds will be simulated. To sync game logic with the
     * physics.
     */
    float nextSimulationStepSeconds(float seconds);
    u32 numNextSimulationSteps(float seconds);

    void addRigidBody(const std::shared_ptr<RigidBody> & body);
    void removeRigidBody(const std::shared_ptr<RigidBody> & body);

    void setGravity(float gravity);

    float update(float seconds);

    void lineCast(const Ray3D &ray,
                  const std::function<bool(const RayCastIntersection &)> &handler
    ) const;

    std::string toString() const;

private:
    void performTimestep(float seconds);
    void integrateTransforms(float seconds);
    void solve();
    void warmStart(Contact & contact) const;
    void solveContactVelocities(Contact & contact);
    void solvePositions(Contact & contact);

private:
    float                           m_timestep              = 1.0f / 60.0f;
    float                           m_timeAccumulator       = 0.0f;
    float                           m_gravity               = 0.0f;
    unsigned int                    m_numVelocityIterations = 6;
    unsigned int                    m_numPositionIterations = 2;
    RigidBodies                     m_rigidBodies;
    std::unique_ptr<Broadphase>     m_broadphase;
    std::unique_ptr<Narrowphase>    m_narrowphase;


};

}

