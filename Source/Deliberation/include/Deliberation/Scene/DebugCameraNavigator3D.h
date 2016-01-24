#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Camera3D;
class InputAdapterBase;

class DELIBERATION_API DebugCameraNavigator3D final
{
public:
    DebugCameraNavigator3D(Camera3D & camera,
                           const InputAdapterBase & inputAdapter,
                           float speed);

    Camera3D & camera() const;
    float speed() const;

    void setSpeed(float speed);

    void update(float seconds);

private:
    Camera3D &                  m_camera;
    const InputAdapterBase &    m_inputAdapter;
    float                       m_speed;
    bool                        m_mousePressed;
    glm::vec2                   m_lastMousePos;
};

}

