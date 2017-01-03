#pragma once

#include <sstream>

#include <glm/gtx/transform.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/Math/GLMUtils.h>
#include <Deliberation/Core/StreamUtils.h>

namespace deliberation
{

inline Pose3D Pose3D::atPosition(const glm::vec3 & position)
{
    Pose3D result;
    result.setPosition(position);
    return result;
}

inline Pose3D Pose3D::atOrientation(const glm::quat & orientation)
{
    Pose3D result;
    result.setOrientation(orientation);
    return result;
}

inline const Pose3D & Pose3D::identity()
{
    static Pose3D i;
    return i;
}

inline Pose3D::Pose3D():
    m_matrixDirty(true),
    m_basisDirty(true)
{
}

inline Pose3D::Pose3D(const glm::vec3 & position, const glm::quat & orientation):
    m_matrixDirty(true),
    m_basisDirty(true),
    m_position(position),
    m_orientation(orientation)
{

}

inline const glm::vec3 & Pose3D::position() const
{
    return m_position;
}

inline const glm::quat & Pose3D::orientation() const
{
    return m_orientation;
}

inline void Pose3D::setPosition(const glm::vec3 & position)
{
    Assert(GLMIsFinite(position), "");

    m_position = position;
    m_matrixDirty = true;
}

inline void Pose3D::setOrientation(const glm::quat & orientation)
{
    Assert(GLMIsFinite(orientation), "");

    m_orientation = orientation;
    m_matrixDirty = true;
    m_basisDirty = true;
}

inline void Pose3D::localTranslate(const glm::vec3 & delta)
{
    m_position += m_orientation * delta;
    m_matrixDirty = true;
}

inline void Pose3D::worldTranslate(const glm::vec3 & delta)
{
    m_position += delta;
    m_matrixDirty = true;
}

// rotate around local axis
inline void Pose3D::localRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = m_orientation * delta;
    m_matrixDirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

inline void Pose3D::worldRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = delta * m_orientation;
    m_matrixDirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

inline const glm::mat3 & Pose3D::basis() const
{
    if (m_basisDirty)
    {
        m_basis = glm::mat3_cast(m_orientation);
    }

    return m_basis;
}

inline const glm::mat4 & Pose3D::matrix() const
{
    if (m_matrixDirty)
    {
        // Yes, this could be done way more efficient...
        m_matrix = glm::translate(m_position) *
                   glm::mat4_cast(m_orientation);
        m_matrixDirty = false;
    }

    return m_matrix;
}

inline glm::vec3 Pose3D::pointLocalToWorld(const glm::vec3 & point) const
{
    return m_position + (m_orientation * point);
}

inline glm::vec3 Pose3D::pointWorldToLocal(const glm::vec3 & point) const
{
    return glm::inverse(m_orientation) * (point - m_position);
}

inline glm::vec3 Pose3D::directionWorldToLocal(const glm::vec3 & dir) const
{
    return glm::inverse(m_orientation) * dir;
}

inline glm::vec3 Pose3D::directionLocalToWorld(const glm::vec3 & dir) const
{
    return m_orientation * dir;
}

inline Pose3D Pose3D::worldTranslated(const glm::vec3 & delta) const
{
    Pose3D result(*this);
    result.worldTranslate(delta);
    return result;
}

inline Pose3D Pose3D::interpolated(const Pose3D & other, float v) const
{
    Pose3D result;

    result.setOrientation(glm::slerp(m_orientation, other.orientation(), v));
    result.setPosition(glm::mix(m_position, other.position(), v));

    return result;
}

inline std::string Pose3D::toString() const
{
    std::stringstream stream;
    stream << "[Position: " << m_position <<
           "; Orientation: " << m_orientation << "]";

    return stream.str();
}

inline bool Pose3D::operator==(const Pose3D &other) const
{
    return m_position == other.m_position &&
           m_orientation == other.m_orientation;
}

inline bool Pose3D::operator!=(const Pose3D &other) const
{
    return !(*this == other);
}

}



