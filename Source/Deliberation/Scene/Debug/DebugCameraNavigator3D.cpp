#include <Deliberation/Scene/Debug/DebugCameraNavigator3D.h>

#include <Deliberation/Core/UpdateFrame.h>
#include <Deliberation/Platform/Input.h>

#include <Deliberation/Scene/Camera3D.h>

#include <Deliberation/Platform/KeyMap.h>

namespace deliberation
{
DebugCameraNavigator3D::DebugCameraNavigator3D(
    Camera3D & camera, const Input & inputAdapter, float speed)
    : m_camera(camera)
    , m_inputAdapter(inputAdapter)
    , m_speed(speed)
    , m_mousePressed(false)
{
}

Camera3D & DebugCameraNavigator3D::camera() const { return m_camera; }

float DebugCameraNavigator3D::speed() const { return m_speed; }

void DebugCameraNavigator3D::setSpeed(float speed) { m_speed = speed; }

void DebugCameraNavigator3D::update(float seconds)
{
    auto fw = glm::normalize(m_camera.orientation() * glm::vec3(0, 0, -1));
    auto up = glm::normalize(m_camera.orientation() * glm::vec3(0, 1, 0));
    auto r = glm::cross(fw, up);

    float x = (m_inputAdapter.keyPressed(Key_A) ? -1 : 0) +
              (m_inputAdapter.keyPressed(Key_D) ? 1 : 0);
    float y = (m_inputAdapter.keyPressed(Key_F) ? -1 : 0) +
              (m_inputAdapter.keyPressed(Key_R) ? 1 : 0);
    float z = (m_inputAdapter.keyPressed(Key_W) ? -1 : 0) +
              (m_inputAdapter.keyPressed(Key_S) ? 1 : 0);

    auto step = m_camera.orientation() * glm::vec3(x, y, z) * m_speed * seconds;

    m_camera.setPosition(m_camera.position() + step);
}

void DebugCameraNavigator3D::onMouseMotion(MouseMotionEvent & event)
{
    if (!event.button(MouseButton::Left)) return;

    double mouseX;
    double mouseY;

    auto mousePos = event.mousePosition();
    auto pressed = m_inputAdapter.mouseButtonDown(MouseButton::Left);

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
        auto delta = event.mouseMovement();
        auto rotation = glm::quat(glm::vec3(delta.y, -delta.x, 0.0f));
        m_camera.setOrientation(m_camera.orientation() * rotation);
    }

    event.consume();
}
}
