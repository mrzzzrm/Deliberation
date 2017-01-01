#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

#include <Deliberation/Core/InputAdapterBase.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

DebugCameraNavigator3D::DebugCameraNavigator3D(Camera3D & camera,
                                               const InputAdapterBase & inputAdapter,
                                               float speed):
    m_camera(camera),
    m_inputAdapter(inputAdapter),
    m_speed(speed),
    m_mousePressed(false)
{

}

Camera3D & DebugCameraNavigator3D::camera() const
{
    return m_camera;
}

float DebugCameraNavigator3D::speed() const
{
    return m_speed;
}

void DebugCameraNavigator3D::setSpeed(float speed)
{
    m_speed = speed;
}

void DebugCameraNavigator3D::update(float seconds)
{
    double mouseX;
    double mouseY;

    auto mousePos = m_inputAdapter.mousePosition();
    auto pressed = m_inputAdapter.mouseButtonDown(InputAdapterBase::MouseButton_Left);

    if (!m_mousePressed && pressed)
    {
        m_mousePressed = true;
    }
    else if (!pressed)
    {
        m_mousePressed = false;
    }
    else
    {
        auto delta = mousePos - m_lastMousePos;
        delta *= 0.004f;
        auto rotation = glm::quat(glm::vec3(-delta.y, -delta.x, 0.0f));
        m_camera.setOrientation(m_camera.orientation() * rotation);
    }

    m_lastMousePos = mousePos;

    auto fw = glm::normalize(m_camera.orientation() * glm::vec3(0, 0, -1));
    auto up = glm::normalize(m_camera.orientation() * glm::vec3(0, 1, 0));
    auto r = glm::cross(fw, up);

    float x = (m_inputAdapter.keyDown(InputAdapterBase::Key_A) ? -1 : 0) + (m_inputAdapter.keyDown(InputAdapterBase::Key_D) ? 1 : 0);
    float y = (m_inputAdapter.keyDown(InputAdapterBase::Key_F) ? -1 : 0) + (m_inputAdapter.keyDown(InputAdapterBase::Key_R) ? 1 : 0);
    float z = (m_inputAdapter.keyDown(InputAdapterBase::Key_W) ? -1 : 0) + (m_inputAdapter.keyDown(InputAdapterBase::Key_S) ? 1 : 0);

    auto step = m_camera.orientation() * glm::vec3(x, y, z) * m_speed * seconds;

    m_camera.setPosition(m_camera.position() + step);
}

}

