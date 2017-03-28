#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Camera3D;
class InputBase;

class DELIBERATION_API DebugCameraNavigator3D final
{
public:
    DebugCameraNavigator3D(Camera3D & camera,
                           const InputBase & inputAdapter,
                           float speed = 10.0f);

    Camera3D & camera() const;
    float speed() const;

    void setSpeed(float speed);

    void update(float seconds);

private:
    Camera3D &                  m_camera;
    const InputBase &    m_inputAdapter;
    float                       m_speed;
    bool                        m_mousePressed;
    glm::vec2                   m_lastMousePos;
};

}

