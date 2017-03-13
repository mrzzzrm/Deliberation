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

    float linearAcceleration() const;
    float angularAcceleration() const;
    const Pose3D & relativePose() const;

    void setLinearAcceleration(float acceleration);
    void setAngularAcceleration(float angularAcceleraton);
    void setRelativePose(const Pose3D & pose);

    void update(const glm::vec3 & position,
                const glm::quat & orientation,
                float seconds);

private:
    Camera3D &  m_camera;
    float       m_linearAcceleration = 750.0f;
    float       m_angularAcceleration = glm::pi<float>() * 10;
    Pose3D      m_relativePose;
};

}