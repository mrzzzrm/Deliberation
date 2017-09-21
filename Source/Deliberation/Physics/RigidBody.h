#pragma once

#include <memory>

#include <BulletDynamics/Dynamics/btRigidBody.h>

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Transform3D.h>
#include <Deliberation/Core/Math/GLMUtils.h>

#include <Deliberation/ECS/Entity.h>

#include <Deliberation/Physics/BulletPhysics.h>
#include <Deliberation/Physics/CollisionShape.h>

class btMotionState;

namespace deliberation
{
struct RigidBodyPayload
{
    virtual ~RigidBodyPayload() = default;
};

class RigidBody final : public std::enable_shared_from_this<RigidBody>
{
public:
    static constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

public:
    RigidBody(
        const std::shared_ptr<CollisionShape> & shape);

    AABB bounds() const { return m_shape->bounds(transform()); }
    const Transform3D & transform() const;

    float mass() const { return inverseMass() == 0.0f ? 0.0f : 1.0f / inverseMass(); }
    float inverseMass() const { return m_btRigidBody->getInvMass(); }

    glm::vec3 linearVelocity() const { return BulletPhysicsConvert(m_btRigidBody->getLinearVelocity()); }
    glm::vec3 angularVelocity() const { return BulletPhysicsConvert(m_btRigidBody->getAngularVelocity()); }

    const std::shared_ptr<CollisionShape> & shape() const { return m_shape; }
    size_t                                  index() const { return m_index; }
    Entity &                                entity() const { return m_entity; }

    std::shared_ptr<btRigidBody> bulletRigidBody() { return m_btRigidBody; }
    const std::shared_ptr<btRigidBody> & bulletRigidBody() const { return m_btRigidBody; }

    glm::vec3 localVelocity(const glm::vec3 & r) const {
        return BulletPhysicsConvert(m_btRigidBody->getVelocityInLocalPoint(BulletPhysicsConvert(r)));
    }

    void setLinearVelocity(const glm::vec3 & velocity) { Assert(GLMIsFinite(velocity)); m_btRigidBody->setLinearVelocity(BulletPhysicsConvert(velocity)); }
    void setAngularVelocity(const glm::vec3 & velocity) { Assert(GLMIsFinite(velocity)); m_btRigidBody->setAngularVelocity(BulletPhysicsConvert(velocity)); }

    void setIndex(size_t index) { m_index = index; }
    void setEntity(Entity entity) { m_entity = entity; }

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setScale(float scale);

//    void setTransform(const Transform3D & transform);

    void applyImpulse(const glm::vec3 & point, const glm::vec3 & impulse) {
        m_btRigidBody->applyImpulse(BulletPhysicsConvert(impulse), BulletPhysicsConvert(point));
    }

    void applyTorqueImpulse(const glm::vec3 & torque) { m_btRigidBody->applyTorqueImpulse(BulletPhysicsConvert(torque)); }
    void applyCentralImpulse(const glm::vec3 & impulse) { m_btRigidBody->applyCentralImpulse(BulletPhysicsConvert(impulse)); }

    // Update the lazy state of the shape and afterwards the center of mass, mass and inertia
    void adaptShape();

    void updateMassAndInertia();
    void updateCenterOfMass();

    void predictTransform(float seconds, Transform3D & prediction) const;

private:
    std::shared_ptr<btRigidBody>        m_btRigidBody;
    size_t                              m_index = INVALID_INDEX;
    std::shared_ptr<CollisionShape>     m_shape;
    std::shared_ptr<btCollisionShape>   m_btCollisionShape;
    std::shared_ptr<btMotionState>      m_btMotionState;
    mutable Entity                      m_entity;
    mutable Transform3D                 m_transform;
};
}
