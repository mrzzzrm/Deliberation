#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/BoxCollisionShape.h>
#include <Deliberation/Physics/RigidBody.h>
#include <Deliberation/Physics/SphereCollisionShape.h>

using namespace deliberation;

class PhysicsExample:
    public Application
{
public:
    PhysicsExample():
        Application("PhysicsExample")
    {
    }

    virtual void onStartup() override
    {
        std::cout << "---- PhysicsExample ----" << std::endl;

        auto shapeA = std::make_shared<SphereCollisionShape>(1.0f);
        auto shapeB = std::make_shared<SphereCollisionShape>(1.5f);
        m_bodyA = std::make_shared<RigidBody>(Transform3D::atPosition({0.0f, 5.0f, 0.0f}), shapeA);
        m_bodyB = std::make_shared<RigidBody>(Transform3D::atPosition({0.0f, 8.0f, 0.0f}), shapeB);

        auto shapeGround = std::make_shared<BoxCollisionShape>(glm::vec3(5.0f, 1.0f, 4.0f));
        m_bodyGround = std::make_shared<RigidBody>(Transform3D::atPosition({0.0f, 0.0f, 0.0f}), shapeGround);

        m_world.setGravity({0.0f, -9.81f, 0.0f});
    }

    virtual void onFrame(float seconds) override
    {
        m_frameIndex++;

        m_world.update(1000.0f / 60.0f);

        std::cout << "Frame " << m_frameIndex << ": " << m_bodyA->transform().toString() <<
            " -- " << m_bodyB->transform().toString() <<
            " -- " << m_bodyGround->transform().toString() << std::endl;

        if (m_frameIndex == 100)
        {
            quit();
        }
    }

private:
    PhysicsWorld                      m_world;
    std::shared_ptr<RigidBody>        m_bodyA;
    std::shared_ptr<RigidBody>        m_bodyB;
    std::shared_ptr<RigidBody>        m_bodyGround;
    int                               m_frameIndex = 0;
};

int main(int argc, char * argv[])
{
    return PhysicsExample().run();
}

