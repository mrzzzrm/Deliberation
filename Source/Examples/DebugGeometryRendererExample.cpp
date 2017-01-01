#include <iostream>
#include <stdlib.h>

#include <Deliberation/Deliberation.h>

#include <Deliberation/Scene/Debug/DebugGeometryManager.h>
#include <Deliberation/Scene/Debug/DebugGeometryRenderer.h>

#include "SceneExampleApplication.h"

using namespace deliberation;

struct Sphere {
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

        for (uint i = 0; i < 14; i++)
        {
            auto & box = m_renderer->addBox({1.2f - i * 0.08f, 0.3f, 1.3f - i * 0.08f}, {1.0f, 0.2f, 0.2f}, false);
            auto transform = Transform3D::atPosition({0.0f, i * box.halfExtent().y, 0.0f});
            transform.setCenter({0.0f, -box.halfExtent().y, 0.0f});
            box.setTransform(transform);
        }

        for (uint i = 0; i < 3; i++) {
            auto index = m_renderer->addSphere({0.0f, 0.5f, 0.8f}, 0.5f).index();

            Sphere sphere;
            sphere.index = index;
            sphere.radius = 3.0f + i * 2.0f;
            sphere.angle = i * 0.6f;
            sphere.y = 0.5f + i * 0.2f;

            m_spheres.push_back(sphere);
        }
    }

    virtual void onFrame(float seconds) override
    {
        SceneExampleApplication::onFrame(seconds);

        for (auto & sphere : m_spheres) {
            auto & s = m_renderer->sphere(sphere.index);
            s.setTransform(Transform3D::atPosition({std::cos(sphere.angle) * sphere.radius,
                                                    sphere.y,
                                                    std::sin(sphere.angle) * sphere.radius}));
        }

        m_renderer->schedule(m_camera);
    }

private:
    Optional<DebugGeometryManager>  m_manager;
    Optional<DebugGeometryRenderer> m_renderer;

    std::vector<Sphere>             m_spheres;
};

int main(int argc, char ** argv)
{
    DebugGeometryRendererExample().run(argc, argv);

    return 0;
}