#include <Deliberation/Scene/Transform3D.h>

#include <cmath>

#include <glm/gtx/transform.hpp>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Core/Math/GLMUtils.h>

namespace deliberation
{

Transform3D::Transform3D():
    m_scale(1.0f),
    m_dirty(true)
{
}

const glm::vec3 & Transform3D::position() const
{
    return m_position;
}

const glm::quat & Transform3D::orientation() const
{
    return m_orientation;
}

const glm::vec3 & Transform3D::center() const
{
    return m_center;
}

float Transform3D::scale() const
{
    return m_scale;
}

void Transform3D::setPosition(const glm::vec3 & position)
{
    Assert(GLMIsFinite(position), "");

    m_position = position;
    m_dirty = true;
}

void Transform3D::setOrientation(const glm::quat & orientation)
{
    Assert(GLMIsFinite(orientation), "");

    m_orientation = orientation;
    m_dirty = true;
}

void Transform3D::setCenter(const glm::vec3 & center)
{
    Assert(GLMIsFinite(center), "");

    m_center = center;
    m_dirty = true;
}

void Transform3D::setScale(float scale)
{
    Assert(std::isfinite(scale), "");
    Assert(scale > 0.0f, "");

    m_scale = scale;
    m_dirty = true;
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
    m_dirty = true;
}

void Transform3D::worldTranslate(const glm::vec3 & delta)
{
    m_position += delta;
    m_dirty = true;
}

// rotate around local axis
void Transform3D::localRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = m_orientation * delta;
    m_dirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

void Transform3D::worldRotate(const glm::quat & delta)
{
    Assert(GLMIsFinite(delta), "");

    m_orientation = delta * m_orientation;
    m_dirty = true;

    Assert(GLMIsFinite(m_orientation), "");
}

const glm::mat4 & Transform3D::matrix() const
{
    if (m_dirty)
    {
        // Yes, this could be done way more efficient...
        m_matrix = glm::translate(m_position) *
                   glm::mat4_cast(m_orientation) *
                   glm::scale(glm::vec3(m_scale)) *
                   glm::translate(-m_center);
        m_dirty = false;
    }

    return m_matrix;
}

glm::vec3 Transform3D::pointLocalToWorld(const glm::vec3 & point) const
{
    return m_position + (m_orientation * (m_scale * (-m_center + point)));
}

glm::vec3 Transform3D::pointWorldToLocal(const glm::vec3 & point) const
{
    return ((glm::inverse(m_orientation) * (point - m_position)) / m_scale) + m_center;
}

glm::vec3 Transform3D::directionWorldToLocal(const glm::vec3 & dir) const
{
    return m_orientation * dir;
}

glm::vec3 Transform3D::directionLocalToWorld(const glm::vec3 & dir) const
{
    return glm::inverse(m_orientation) * dir;
}

Transform3D Transform3D::interpolated(const Transform3D & other, float v) {
    Transform3D result;

    result.setOrientation(glm::slerp(m_orientation, other.orientation(), v));
    result.setPosition(glm::mix(m_position, other.position(), v));
    result.setScale(glm::mix(m_scale, other.scale(), v));
    result.setCenter(glm::mix(m_center, other.center(), v));

    return result;
}

bool Transform3D::operator==(const Transform3D &other) const
{
    return m_position == other.m_position &&
           m_orientation == other.m_orientation &&
           m_center == other.m_center &&
           m_scale == other.m_scale;
}

bool Transform3D::operator!=(const Transform3D &other) const
{
    return !(*this == other);
}

}



