#include <iostream>
#include <stdlib.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "SceneExampleApplication.h"

using namespace deliberation;

struct OrbittingSphere {
    size_t index = -1;
    float radius = 0.0f;
    float angle = 0.0f;
    float y = 0.0f;
};

class DebugGeometryRendererExample:
    public SceneExampleApplication
{
public:
    DebugGeometryRendererExample():
        SceneExampleApplication("DebugGeometryRendererExample")
    {

    }

    virtual void onStartup() override
    {
        SceneExampleApplication::onStartup();

        m_camera.setPosition({0.0f, 3.0f, 6.0f});

        m_manager.reset(context());
        m_renderer.reset(*m_manager);

        for (uint i = 0; i < 5; i++)
        {
            auto & box = m_renderer->addBox({1.2f - i * 0.18f, 0.3f, 1.3f - i * 0.18f}, {1.0f, 0.2f, 0.2f}, false);
            auto transform = Transform3D::atPosition({0.0f, i * (2.0f * box.halfExtent().y), 0.0f});
            transform.setCenter({0.0f, -box.halfExtent().y, 0.0f});
            box.setTransform(transform);
        }

        m_renderer->addArrow({}, {}, {0.0f, 1.0f, 0.0f});

        for (uint i = 0; i < 3; i++) {
            auto index = m_renderer->addSphere({0.0f, 0.5f, 0.8f}, 0.5f).index();
            m_renderer->addPoint({}, {0.0f, 0.5f, 0.8f});

            OrbittingSphere sphere;
            sphere.index = index;
            sphere.radius = 3.0f + i * 2.0f;
            sphere.angle = i * 0.6f;
            sphere.y = 0.5f + i * 0.2f;

            m_spheres.push_back(sphere);
        }

        m_renderer->addPose(Pose3D({2.0f, 0.2f, -5.0f}, glm::quat()));

        std::vector<BasicVertex> wireframeMesh(45);
        for (uint s = 0; s < wireframeMesh.size(); s++) {
            auto angle = s * glm::pi<float>() / 7.0f;
            auto radius = 5.0f - s * 0.2f;

            wireframeMesh[s].position.x = std::cos(angle) * radius;
            wireframeMesh[s].position.y = s * 0.2f;
            wireframeMesh[s].position.z = std::sin(angle) * radius;
        }
        auto & wireframe = m_renderer->addWireframe({0.5f, 0.5f, 0.5f});
        wireframe.addLineStrip(wireframeMesh);
    }

    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);

        for (auto & sphere : m_spheres) {
            sphere.angle += seconds / sphere.radius;
            auto & s = m_renderer->sphere(sphere.index);
            auto & p = m_renderer->point(sphere.index);

            auto spherePosition = glm::vec3{std::cos(sphere.angle) * sphere.radius,
                                            sphere.y,
                                            std::sin(sphere.angle) * sphere.radius};

            s.setTransform(Transform3D::atPosition(spherePosition));
            p.setTransform(Transform3D::atPosition(spherePosition + glm::vec3(0.0f, 2.0f, 0.0f)));
        }

        m_arrowPhase += seconds;
        m_renderer->arrow(0).setOrigin({1.5f, 0.0f, 1.5f});
        m_renderer->arrow(0).setDelta({0.0f, 0.6f, 0.0f});

        m_wireframeAngle -= seconds;
        m_renderer->wireframe(0).setTransform(Transform3D::atOrientation(glm::quat({0.0f, m_wireframeAngle, 0.0f})));

        m_renderer->schedule(m_camera);
    }

private:
    Optional<DebugGeometryManager>  m_manager;
    Optional<DebugGeometryRenderer> m_renderer;

    std::vector<OrbittingSphere>    m_spheres;

    float                           m_arrowPhase = 0.0f;
    float                           m_wireframeAngle = 0.0f;
};

int main(int argc, char ** argv)
{
    DebugGeometryRendererExample().run(argc, argv);

    return 0;
}