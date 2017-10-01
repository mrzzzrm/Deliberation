#include <Deliberation/Physics/PhysicsWorld.h>

#include <iostream>
#include <sstream>

#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>

#include <Deliberation/Core/Math/MathUtils.h>
#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Scope.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Physics/CollisionShape.h>
#include <Deliberation/Physics/RigidBody.h>

namespace deliberation
{
PhysicsWorld::PhysicsWorld(float timestep) : m_timestep(timestep)
{
    m_collisionConfiguration = std::make_shared<btDefaultCollisionConfiguration>();
    m_collisionDispatcher = std::make_shared<btCollisionDispatcher>(m_collisionConfiguration.get());
    m_constraintSolver = std::make_shared<btSequentialImpulseConstraintSolver>();
    m_broadphase = std::make_shared<btDbvtBroadphase>();
    m_dynamicsWorld = std::make_shared<btDiscreteDynamicsWorld>(m_collisionDispatcher.get(),
                                                                m_broadphase.get(),
                                                                m_constraintSolver.get(),
                                                                m_collisionConfiguration.get());
    m_dynamicsWorld->setGravity(btVector3(0.0f, 0.0f, 0.0f));
}


float PhysicsWorld::probeNextSimulationStepSeconds(float seconds) const
{
    const auto nextTimeAccumulator = m_timeAccumulator + seconds;

    if (nextTimeAccumulator >= m_timestep) {
        return int(nextTimeAccumulator / m_timestep) * m_timestep;
    } else {
        return 0.0f;
    }
}


void PhysicsWorld::setGravity(const glm::vec3 & gravity)
{
    m_dynamicsWorld->setGravity(BulletPhysicsConvert(gravity));
}

void PhysicsWorld::addRigidBody(const std::shared_ptr<RigidBody> & body)
{
    auto index = m_rigidBodies.insert(body);
    body->setIndex(index);

    m_dynamicsWorld->addRigidBody(body->bulletRigidBody().get());
}

void PhysicsWorld::removeRigidBody(const std::shared_ptr<RigidBody> & body)
{
    auto index = body->index();
    AssertM(
        index != RigidBody::INVALID_INDEX,
        "Body was never added to PhysicsWorld");

    m_rigidBodies.erase(index);

    body->setIndex(RigidBody::INVALID_INDEX);

    m_dynamicsWorld->removeRigidBody(body->bulletRigidBody().get());
}

float PhysicsWorld::update(float seconds)
{
    DELIBERATION_SCOPE("PhysicsWorld::update()");

    for (auto & body : m_rigidBodies)
    {
        body->adaptShape();
    }

    const auto numPerformedSteps = [&] () {
        DELIBERATION_SCOPE("btDiscreteDynamicsWorld::stepSimulation");
        return m_dynamicsWorld->stepSimulation(seconds, 10, m_timestep);
    } ();
    const auto simulatedSeconds = numPerformedSteps * m_timestep;

    m_numSimulatedSteps += numPerformedSteps;
    m_timeAccumulator += seconds;
    m_timeAccumulator -= simulatedSeconds;

    return simulatedSeconds;
}

void PhysicsWorld::lineCast(
    const Ray3D &                                            ray,
    const std::function<bool(const RayCastIntersection &)> & handler) const
{

    struct Callback : public btBroadphaseRayCallback
    {
        const PrimitiveTester &                                  m_primitiveTester;
        const Ray3D &                                            m_ray;
        const std::function<bool(const RayCastIntersection &)> & m_handler;

        Callback(const Ray3D &                                            ray,
                 const std::function<bool(const RayCastIntersection &)> & handler,
                 const PrimitiveTester & primitiveTester):
            m_ray(ray), m_handler(handler), m_primitiveTester(primitiveTester)
        {
            const auto normalizedDirection = glm::normalize(ray.direction());

            ///what about division by zero? --> just set rayDirection[i] to INF/BT_LARGE_FLOAT
            m_rayDirectionInverse[0] = normalizedDirection[0] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / normalizedDirection[0];
            m_rayDirectionInverse[1] = normalizedDirection[1] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / normalizedDirection[1];
            m_rayDirectionInverse[2] = normalizedDirection[2] == btScalar(0.0) ? btScalar(BT_LARGE_FLOAT) : btScalar(1.0) / normalizedDirection[2];
            m_signs[0] = m_rayDirectionInverse[0] < 0.0;
            m_signs[1] = m_rayDirectionInverse[1] < 0.0;
            m_signs[2] = m_rayDirectionInverse[2] < 0.0;

            m_lambda_max = glm::dot(normalizedDirection, m_ray.direction());
        }



        virtual bool process(const btBroadphaseProxy* proxy)
        {
            auto * bulletBody = (btRigidBody*)proxy->m_clientObject;
            auto * body = (RigidBody*)bulletBody->getUserPointer();

            m_primitiveTester.lineTest(m_ray, body->shared_from_this(), m_handler);

            return true;
        }
    };


    const auto bulletFrom = BulletPhysicsConvert(ray.origin());
    const auto bulletTo = BulletPhysicsConvert(ray.origin() + ray.direction());

    Callback callback(
        ray, handler, m_primitiveTester
    );

    m_broadphase->rayTest(
        bulletFrom,
        bulletTo,
        callback
    );
}
}
