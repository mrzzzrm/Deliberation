#include <iostream>
#include <chrono>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

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

        m_camera.setPosition({0.0f, 5.0f, 25.0f});
        m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
        m_camera.setAspectRatio((float)context().backbuffer().width() / (float)context().backbuffer().height());

        m_clear = context().createClear();
        m_navigator.reset(m_camera, inputAdapter(), 10.0f);
        m_geometryRenderer.reset(context(), m_camera);

        auto shapeA = std::make_shared<BoxShape>(glm::vec3{5.0f, 1.0f, 1.0f});
        auto shapeGround = std::make_shared<BoxShape>(glm::vec3{10.0f, 1.0f, 6.0f});

        for (int i = 0; i < 1; i++) {
            auto body = std::make_shared<RigidBody>(shapeA, Transform3D::atPosition({0.0f, 3.0f + 6.5f * i, 0.0f}));
            body->setLinearVelocity({0.0f, -2.0f  , 0.0f});
            m_world.addRigidBody(body);
            m_bodies.push_back(body);
        }

        m_bodyGround = std::make_shared<RigidBody>(shapeGround, Transform3D::atPosition({0.0f, 0.0f, 0.0f}));
        m_bodyGround->setMass(50000);

        m_world.setGravity(0.0f);

        m_world.addRigidBody(m_bodyGround);

        std::vector<glm::vec3> colors({
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        });

        for (auto i = 0; i < m_bodies.size(); i++)
        {
            auto index = m_geometryRenderer.get().addBox(shapeA->halfExtent(), colors[i % colors.size()], false);
            m_bodyIndices.push_back(index);
        }

        m_bodyGroundIndex = m_geometryRenderer.get().addBox(shapeGround->halfExtent(), {0.0f, 1.0f, 0.0f}, false);
    }

    virtual void onFrame(float seconds) override
    {
       // std::cout << seconds << std::endl;

        if (inputAdapter().keyDown(InputAdapterBase::Key_SPACE) || inputAdapter().keyPressed(InputAdapterBase::Key_RIGHT))
        {
            m_world.update(seconds);
        }

        m_navigator.get().update(seconds);

        for (auto i = 0; i < m_bodies.size(); i++)
        {
            m_geometryRenderer.get().box(m_bodyIndices[i]).transform() = m_bodies[i]->transform();
        }

        m_geometryRenderer.get().box(m_bodyGroundIndex).transform() = m_bodyGround->transform();

        m_clear.schedule();
        m_grid.get().draw();
        m_geometryRenderer.get().schedule();
    }

private:
    PhysicsWorld                      m_world;
    std::vector<
        std::shared_ptr<RigidBody>>   m_bodies;
    std::shared_ptr<RigidBody>        m_bodyGround;

    Clear                             m_clear;
    Optional<DebugGrid3DRenderer>     m_grid;
    Camera3D                          m_camera;
    Optional<DebugCameraNavigator3D>  m_navigator;
    Optional<DebugGeometryRenderer>   m_geometryRenderer;

    std::vector<uint>                 m_bodyIndices;
    uint                              m_bodyBIndex;
    uint                              m_bodyGroundIndex;


    std::chrono::steady_clock::time_point m_physicsPlayStart;
};

int main(int argc, char * argv[])
{
    return PhysicsExample().run();
}

