#pragma once

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Camera3D;

class DELIBERATION_API CameraDolly3D
{
public:
    CameraDolly3D(Camera3D & camera);

    void update(const glm::vec3 & position, const glm::quat & orientation, float seconds);

private:
    Camera3D & m_camera;
};

}