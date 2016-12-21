#include <Deliberation/Scene/CameraDolly3D.h>

namespace deliberation
{

CameraDolly3D::CameraDolly3D(Camera3D & camera):
    m_camera(camera)
{

}

void CameraDolly3D::update(const glm::vec3 & position, const glm::quat & orientation, float seconds)
{

}

}