#pragma once

#include <Deliberation/Core/Math/Pose3D.h>

#include <Deliberation/Deliberation.h>

namespace deliberation
{

class Camera3D;

class DELIBERATION_API CameraDolly3D
{
public:
    CameraDolly3D(Camera3D & camera);

    float acceleration() const;
    const Pose3D & relativePose() const;

    void setAcceleration(float acceleration);
    void setRelativePose(const Pose3D & pose);

    void update(const glm::vec3 & position,
                const glm::quat & orientation,
                float seconds);

private:
    Camera3D &  m_camera;
    float       m_acceleration = 50.0f;
    Pose3D      m_relativePose;
};

}