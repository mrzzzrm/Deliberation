#include "SceneExampleApplication.h"

using namespace deliberation;

SceneExampleApplication::SceneExampleApplication(const std::string & name):
    Application(name)
{

}

void SceneExampleApplication::onStartup()
{
    m_ground.reset(context(), m_camera);
    m_ground->setSize(30.0f);
    m_ground->setRadius(10.0f);

    m_camera.setPosition({0.0f, 1.0f, 3.0f});
    m_camera.setOrientation(glm::quat({-0.2f, 0.0f, 0.0f}));
    m_camera.setAspectRatio((float)context().backbuffer().width() / context().backbuffer().height());

    m_clear = context().createClear();

    m_navigator.reset(m_camera, inputAdapter(), 10.0f);
}

void SceneExampleApplication::onFrame(float seconds)
{
    m_navigator->update(seconds);

    m_clear.schedule();

    if (m_ground.engaged())
    {
        m_ground->schedule();
    }
}