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
#include <Deliberation/Scene/DebugPhysicsWorldRenderer.h>

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
        m_geometryRenderer2.reset(context(), m_camera);
        m_world.reset();
        m_worldDebugRenderer.reset(context(), m_world.get(), m_camera);

        auto shapeA = std::make_shared<BoxShape>(glm::vec3{5.0f, 1.0f, 1.0f});
        auto shapeGround = std::make_shared<BoxShape>(glm::vec3{10.0f, 1.0f, 6.0f});

        for (int i = 0; i < 1; i++) {
            auto body = std::make_shared<RigidBody>(shapeA, Transform3D::atPosition({12.0f, 3.0f + 6.5f * i, 0.0f}));
            body->setLinearVelocity({0.0f, -10.0f  , 0.0f});
            //body->setAngularVelocity({1.24698,2.5979,-1.47831});
            m_world.get().addRigidBody(body);
            m_bodies.push_back(body);
        }

        m_bodyGround = std::make_shared<RigidBody>(shapeGround, Transform3D::atPosition({0.0f, 0.0f, 0.0f}));
        m_bodyGround->setMass(50000);

        m_world.get().setGravity(0.0f);

        m_world.get().addRigidBody(m_bodyGround);

        std::vector<glm::vec3> colors({
            {1.0f, 0.0f, 0.0f},
            {1.0f, 1.0f, 0.0f},
            {1.0f, 0.0f, 1.0f},
            {1.0f, 1.0f, 1.0f}
        });

        for (auto i = 0; i < m_bodies.size(); i++)
        {
            auto index = m_geometryRenderer.get().addBox(shapeA->halfExtent(), colors[i % colors.size()], true);
            m_bodyIndices.push_back(index);
        }

        m_bodyGroundIndex = m_geometryRenderer.get().addBox(shapeGround->halfExtent(), {0.0f, 1.0f, 0.0f}, true);
    }

    virtual void onFrame(float seconds) override
    {
//        auto p = glm::vec3{-2,-2,1};
//        auto v = m_bodies[0]->localVelocity(p);
//        std::cout << m_bodies[0]->angularVelocity() << " " << v << std::endl;

//        m_geometryRenderer2->resizeArrows(1, {1.0f, 1.0f, 1.0f}, true);
//        m_geometryRenderer2->arrow(0).reset(p + m_bodies[0]->transform().position(), v);


//        for (auto & body : m_bodies)
//        {
//            std::cout << body->transform().position() << body->transform().orientation() << " - ";
//        }
//        std::cout << m_bodyGround->transform().position() << m_bodyGround->transform().orientation() << std::endl;

        if (inputAdapter().keyPressed(InputAdapterBase::Key_RIGHT))
        {
            m_world.get().update(m_world.get().timestep());
        }
        if (inputAdapter().keyDown(InputAdapterBase::Key_SPACE))
        {
            m_world.get().update(seconds);
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
        m_geometryRenderer2.get().schedule();
        m_worldDebugRenderer.get().schedule();
    }

private:
    Optional<PhysicsWorld>              m_world;
    Optional<DebugPhysicsWorldRenderer> m_worldDebugRenderer;
    std::vector<
        std::shared_ptr<RigidBody>>     m_bodies;
    std::shared_ptr<RigidBody>          m_bodyGround;

    Clear                               m_clear;
    Optional<DebugGrid3DRenderer>       m_grid;
    Camera3D                            m_camera;
    Optional<DebugCameraNavigator3D>    m_navigator;
    Optional<DebugGeometryRenderer>     m_geometryRenderer;
    Optional<DebugGeometryRenderer>     m_geometryRenderer2;

    std::vector<uint>                   m_bodyIndices;
    uint                                m_bodyGroundIndex;
};

void TestAngularVelocity(const glm::vec3 & w, const glm::vec3 & r) {
    auto v = glm::cross(w, r);

    auto f = 0.001f;

    auto angle = glm::length(w * f);
    auto axis = glm::normalize(w * f);

    auto quat = glm::rotate(glm::quat(), angle, axis);

    auto i = quat * r;

    std::cout << v << " " << ((i - r)/f) << std::endl;
}

void AngularVelocitySandbox() {
    TestAngularVelocity({0,1,0}, {1,0,0});
    TestAngularVelocity({1.24698,2.5979,-1.47831}, {-2,-2,1});
    TestAngularVelocity({1.24698,2.5979,-1.47831}, {-2.04013,-1.87571,0.999999});
}

int main(int argc, char * argv[])
{
    AngularVelocitySandbox();
  //  return 0;
  //  return PhysicsExample().run(argc, argv);
}

