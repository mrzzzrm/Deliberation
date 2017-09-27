#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Platform/MouseMotionEvent.h>

namespace deliberation
{
class Camera3D;
class InputManager;
class UpdateFrame;

class DebugCameraNavigator3D final
{
public:
    DebugCameraNavigator3D(
        Camera3D & camera, float speed = 10.0f);

    Camera3D & camera() const;
    float      speed() const;

    void setSpeed(float speed);

    void update(float seconds);
    void onMouseMotion(MouseMotionEvent & event);

private:
    Camera3D &    m_camera;
    float         m_speed;
    bool          m_mousePressed;
};
}
