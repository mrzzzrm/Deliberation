#include <Deliberation/Core/Math/Transform3D.h>

#include <cmath>
#include <sstream>

#include <glm/gtx/transform.hpp>

#include <Deliberation/Core/Assert.h>
#include <Deliberation/Core/StreamUtils.h>

#include <Deliberation/Core/Math/GLMUtils.h>
#include <Deliberation/Core/Math/Pose3D.h>

namespace deliberation
{
Transform3D Transform3D::atPosition(const glm::vec3 & position)
{
    Transform3D result;
    result.setPosition(position);
    return result;
}

Transform3D Transform3D::atOrientation(const glm::quat & orientation)
{
    Transform3D result;
    result.setOrientation(orientation);
    return result;
}

Transform3D Transform3D::withScale(float scale)
{
    Transform3D result;
    result.setScale(scale);
    return result;
}

const Transform3D & Transform3D::identity()
{
    static Transform3D i;
    return i;
}

Transform3D Transform3D::fromPose(const Pose3D & pose)
{
    Transform3D transform3D;
    transform3D.setPosition(pose.position());
    transform3D.setOrientation(pose.orientation());
    transform3D.setCenter(pose.center());
    return transform3D;
}

Transform3D::Transform3D()
    : m_scale(1.0f), m_matrixDirty(true), m_basisDirty(true)
{
}

const glm::vec3 & Transform3D::position() const { return m_position; }

const glm::quat & Transform3D::orientation() const { return m_orientation; }

const glm::vec3 & Transform3D::center() const { return m_center; }

float Transform3D::scale() const { return m_scale; }

void Transform3D::setPosition(const glm::vec3 & position)
{
    Assert(GLMIsFinite(position), "");

    m_position = position;
    m_matrixDirty = true;
}

void Transform3D::setOrientation(const glm::quat & orientation)
{
    Assert(GLMIsFinite(orientation), "");

    m_orientation = orientation;
    m_matrixDirty = true;
    m_basisDirty = true;
}

void Transform3D::setCenter(const glm::vec3 & center)
{
    Assert(GLMIsFinite(center), "");

    m_center = center;
    m_matrixDirty = true;
}

void Transform3D::setScale(float scale)
{
    Assert(std::isfinite(scale), "");
    Assert(scale > 0.0f, "");

    m_scale = scale;
    m_matrixDirty = true;
}

void Transform3D::setCenterAndAdjustPosition(const glm::vec3 & center)
{
    auto oldOrigin = pointLocalToWorld(glm::vec3(0.0f, 0.0f, 0.0f));
    setCenter(center);
    auto newOrigin = pointLocalToWorld(glm::vec3(0.0f, 0.0f, 0.0f));
    setPosition(m_position + (oldOrigin - newOrigin));
}

void Transform3D::localTranslate(const glm::vec3 & delta)
{
    m_position += m_orientation * delta;
    m_matrixDirty = true;
}

void Transform3D::worldTranslate(const glm::vec3 & delta)
{
    m_position += delta;
    m_matrixDirty = true;
}

// rotate around local axis
void Transform3D::localRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = m_orientation * delta;
    m_matrixDirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

void Transform3D::worldRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = delta * m_orientation;
    m_matrixDirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

const glm::mat3 & Transform3D::basis() const
{
    if (m_basisDirty)
    {
        m_basis = glm::mat3_cast(m_orientation);
    }

    return m_basis;
}

const glm::mat4 & Transform3D::matrix() const
{
    if (m_matrixDirty)
    {
        // Yes, this could be done way more efficient...
        m_matrix = glm::translate(m_position) * glm::mat4_cast(m_orientation) *
                   glm::scale(glm::vec3(m_scale)) * glm::translate(-m_center);
        m_matrixDirty = false;
    }

    return m_matrix;
}

glm::vec3 Transform3D::pointLocalToWorld(const glm::vec3 & point) const
{
    return m_position + (m_orientation * (m_scale * (point - m_center)));
}

glm::vec3 Transform3D::pointWorldToLocal(const glm::vec3 & point) const
{
    return ((glm::inverse(m_orientation) * (point - m_position)) / m_scale) +
           m_center;
}

glm::vec3 Transform3D::directionWorldToLocal(const glm::vec3 & dir) const
{
    return glm::inverse(m_orientation) * dir;
}

glm::vec3 Transform3D::directionLocalToWorld(const glm::vec3 & dir) const
{
    return m_orientation * dir;
}

Transform3D Transform3D::scaled(float scale) const
{
    Transform3D result(*this);
    result.setScale(result.scale() * scale);
    return result;
}

Transform3D Transform3D::worldTranslated(const glm::vec3 & delta) const
{
    Transform3D result(*this);
    result.worldTranslate(delta);
    return result;
}

Transform3D Transform3D::interpolated(const Transform3D & other, float v) const
{
    Transform3D result;

    result.setOrientation(glm::slerp(m_orientation, other.orientation(), v));
    result.setPosition(glm::mix(m_position, other.position(), v));
    result.setScale(glm::mix(m_scale, other.scale(), v));
    result.setCenter(glm::mix(m_center, other.center(), v));

    return result;
}

std::string Transform3D::toString() const
{
    std::stringstream stream;
    stream << "[Position: " << m_position << "; Orientation: " << m_orientation
           << "; Center: " << m_center << "; Scale: " << m_scale << "]";

    return stream.str();
}

bool Transform3D::operator==(const Transform3D & other) const
{
    return m_position == other.m_position &&
           m_orientation == other.m_orientation && m_center == other.m_center &&
           m_scale == other.m_scale;
}

bool Transform3D::operator!=(const Transform3D & other) const
{
    return !(*this == other);
}
}
