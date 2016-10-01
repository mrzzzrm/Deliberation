#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Platform/Application.h>

#include <Deliberation/Physics/PhysicsWorld.h>
#include <Deliberation/Physics/BoxShape.h>
#include <Deliberation/Physics/RigidBody.h>

#include <Deliberation/Scene/Camera3D.h>
#include <Deliberation/Scene/DebugGeometryRenderer.h>
#include <Deliberation/Scene/DebugGrid3DRenderer.h>
#include <Deliberation/Scene/DebugCameraNavigator3D.h>

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

        m_grid.reset(context(), 2.5f, m_camera);

        m_camera.setPosition({0.0f, 5.0f, 15.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / (float)context().backbuffer().height());

        m_clear = context().createClear();
        m_navigator.reset(m_camera, inputAdapter(), 10.0f);
        m_geometryRenderer.reset(context(), m_camera);

        auto shapeA = std::make_shared<BoxShape>(glm::vec3{1.0f, 1.0f, 1.0f});
        auto shapeGround = std::make_shared<BoxShape>(glm::vec3{10.0f, 1.0f, 10.0f});

        m_bodyA = std::make_shared<RigidBody>(shapeA, Transform3D::atPosition({0.0f, 15.0f, 0.0f}));
        m_bodyGround = std::make_shared<RigidBody>(shapeGround, Transform3D::atPosition({0.0f, 0.0f, 0.0f}));
        m_bodyGround->setStatic(true);

        m_world.setGravity(2.0f);

        m_world.addRigidBody(m_bodyA);
        m_world.addRigidBody(m_bodyGround);

        m_bodyAIndex = m_geometryRenderer.get().addBox(shapeA->halfExtent(), {1.0f, 0.0f, 0.0f}, false);
        m_bodyGroundIndex = m_geometryRenderer.get().addBox(shapeGround->halfExtent(), {1.0f, 0.0f, 0.0f}, false);
    }

    virtual void onFrame(float seconds) override
    {
        m_world.update(0.017f);

        std::cout << m_bodyA->transform().position() << " " << m_bodyA->linearVelocity() << std::endl;

        m_navigator.get().update(seconds);

        m_geometryRenderer.get().box(m_bodyAIndex).transform() = m_bodyA->transform();
        m_geometryRenderer.get().box(m_bodyGroundIndex).transform() = m_bodyGround->transform();

        m_clear.schedule();
        m_grid.get().draw();
        m_geometryRenderer.get().schedule();
    }

private:
    PhysicsWorld                      m_world;
    std::shared_ptr<RigidBody>        m_bodyA;
    std::shared_ptr<RigidBody>        m_bodyGround;

    Clear                             m_clear;
    Optional<DebugGrid3DRenderer>     m_grid;
    Camera3D                          m_camera;
    Optional<DebugCameraNavigator3D>  m_navigator;
    Optional<DebugGeometryRenderer>   m_geometryRenderer;

    uint                              m_bodyAIndex;
    uint                              m_bodyGroundIndex;
};

int main(int argc, char * argv[])
{
    return PhysicsExample().run();
}

