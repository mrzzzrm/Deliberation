#include <Deliberation/Scene/CameraDolly3D.h>

#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{

CameraDolly3D::CameraDolly3D(Camera3D & camera):
    m_camera(camera)
{
}

float CameraDolly3D::acceleration() const
{
    return m_acceleration;
}

const Pose3D & CameraDolly3D::relativePose() const
{
    return m_relativePose;
}

void CameraDolly3D::setAcceleration(float acceleration)
{
    m_acceleration = acceleration;
}

void CameraDolly3D::setRelativePose(const Pose3D &pose)
{
    m_relativePose = pose;
}

void CameraDolly3D::update(const glm::vec3 & position,
            const glm::quat & orientation,
            float seconds)
{
    Assert(m_acceleration >= 0.0f, "Invalid acceleration");

    static float lastDistance = 0.0f;

    /**
     * Move position
     */
    const auto currentPositionDelta = position - m_camera.position();
    const auto currentDistance = glm::length(currentPositionDelta);

    if (currentDistance != 0.0f)
    {
        const auto direction = glm::normalize(currentPositionDelta);

        const auto t0 = std::sqrt(2 * currentDistance / m_acceleration);

        if (seconds >= t0)
        {
            m_camera.setPosition(position);
        }
        else
        {
            const auto t1 = t0 - seconds;
            const auto deltaDistance = currentDistance - (m_acceleration / 2.0f) * (t1 * t1);

            m_camera.setPosition(m_camera.position() + direction * deltaDistance);
        }
    }


    /**
     * Move orientation
     */
    m_camera.setOrientation(glm::mix(m_camera.orientation(), orientation, 2.0f * seconds));

}

}