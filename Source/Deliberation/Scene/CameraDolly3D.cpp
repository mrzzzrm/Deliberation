#include <Deliberation/Scene/CameraDolly3D.h>

#include <Deliberation/Core/Math/FloatUtils.h>
#include <Deliberation/Scene/Camera3D.h>

namespace deliberation
{
CameraDolly3D::CameraDolly3D(Camera3D & camera) : m_camera(camera) {}

float CameraDolly3D::linearAcceleration() const { return m_linearAcceleration; }

float CameraDolly3D::angularAcceleration() const
{
    return m_angularAcceleration;
}

const Pose3D & CameraDolly3D::relativePose() const { return m_relativePose; }

void CameraDolly3D::setLinearAcceleration(float acceleration)
{
    m_linearAcceleration = acceleration;
}

void CameraDolly3D::setAngularAcceleration(float angularAcceleration)
{
    m_angularAcceleration = angularAcceleration;
}

void CameraDolly3D::setRelativePose(const Pose3D & pose)
{
    m_relativePose = pose;
}

void CameraDolly3D::update(
    const glm::vec3 & position, const glm::quat & orientation, float seconds)
{
    AssertM(m_linearAcceleration >= 0.0f, "Invalid acceleration");

    /**
     * Move position
     */
    const auto currentPositionDelta = position - m_camera.position();
    const auto currentDistance = glm::length(currentPositionDelta);

    if (currentDistance != 0.0f)
    {
        const auto direction = glm::normalize(currentPositionDelta);

        const auto t0 = std::sqrt(2 * currentDistance / m_linearAcceleration);

        if (seconds >= t0)
        {
            m_camera.setPosition(position);
        }
        else
        {
            const auto t1 = t0 - seconds;
            const auto deltaDistance =
                currentDistance - (m_linearAcceleration / 2.0f) * (t1 * t1);

            m_camera.setPosition(
                m_camera.position() + direction * deltaDistance);
        }
    }

    /**
     * Move orientation
     */
    {
        auto o = orientation;

        const auto diff = o * glm::inverse(m_camera.orientation());

        // sometimes the angle will snap from nearly 0 to nearly 2*pi, for whatever reason... we always want the shorter angle
        auto angle = glm::angle(diff);
        if (angle > glm::pi<float>()) angle = glm::two_pi<float>() - angle;

        if (angle == angle) // Is this supposed to be NaN detection?
        {
            const auto t0 = std::sqrt(2 * angle / m_angularAcceleration);
            if (seconds >= t0)
            {
                m_camera.setOrientation(o);
            }
            else
            {
                m_camera.setOrientation(glm::slerp(
                    m_camera.orientation(), o, seconds / t0));
            }
        }
    }
}
}