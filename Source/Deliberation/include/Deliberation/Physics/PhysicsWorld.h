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
    PhysicsWorld(float timestep = 1.0f / 60.0f);

    void addRigidBody(std::shared_ptr<RigidBody> & body);
    void removeRigidBody(std::shared_ptr<RigidBody> & body);

    void setGravity(float gravity);

    void update(float seconds);

    std::string toString() const;

private:
    void performTimestep(float seconds);
    void integrateTransforms(float seconds);

private:
    float                                       m_timestep  = 1.0f / 60.0f;
    float                                       m_gravity   = 0.0f;
    SparseVector<std::shared_ptr<RigidBody>>    m_rigidBodies;
};

}

