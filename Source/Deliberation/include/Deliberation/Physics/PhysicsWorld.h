#pragma once

#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

#include <Deliberation/Core/SparseVector.h>

namespace deliberation
{

class RigidBody;

class DELIBERATION_API PhysicsWorld final
{
public:
    PhysicsWorld();

    void addRigidBody(const std::shared_ptr<RigidBody> & body);
    void removeRigidBody(const std::shared_ptr<RigidBody> & body);

    void setGravity(const glm::vec3 & gravity);

    void update(float seconds);

private:
    SparseVector<std::shared_ptr<RigidBody>> m_rigidBodies;
};

}

