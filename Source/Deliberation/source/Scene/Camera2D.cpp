#include "Camera2D.h"

#include <glm/gtc/matrix_transform.hpp>

Camera2D::Camera2D():
    m_scale(1.0f),
    m_center(0.0f, 0.0f),
    m_aspectRatio(1.0f),
    m_dirty(true)
{

}

Camera2D::Camera2D(float scale, const glm::vec2 & center, float aspectRatio):
    m_scale(scale),
    m_center(center),
    m_aspectRatio(aspectRatio),
    m_dirty(true)
{

}

float Camera2D::scale() const
{
    return m_scale;
}

const glm::vec2 & Camera2D::center() const
{
    return m_center;
}

float Camera2D::aspectRatio() const
{
    return m_aspectRatio;
}

void Camera2D::setScale(float scale)
{
    m_scale = scale;
}

void Camera2D::setCenter(const glm::vec2 & center)
{
    m_center = center;
    m_dirty = true;
}

void Camera2D::setAspectRatio(float aspectRatio)
{
    m_aspectRatio = aspectRatio;
    m_dirty = true;
}

const glm::quat & Camera2D::orientation() const
{
    return m_orientation;
}

const glm::mat4 & Camera2D::view() const
{
    if (m_dirty)
    {
        update();
    }

    return m_view;
}

const glm::mat4 & Camera2D::projection() const
{
    if (m_dirty)
    {
        update();
    }

    return m_projection;
}

const glm::mat4 & Camera2D::viewProjection() const
{
    if (m_dirty)
    {
        update();
    }

    return m_viewProjection;
}

const glm::mat4 & Camera2D::viewProjectionInverted() const
{
    if (m_dirty)
    {
        update();
    }

    return m_viewProjectionInverted;
}

void Camera2D::update() const
{
    if (!m_dirty)
    {
        return;
    }

    m_view = glm::translate(glm::mat4(1.0f), -glm::vec3(m_center, 0.0f));
    m_projection = glm::scale(glm::mat4(1.0f), glm::vec3(m_scale / m_aspectRatio, m_scale, 1.0f));
    m_viewProjection = m_projection * m_view;
    m_viewProjectionInverted = glm::inverse(m_viewProjection);

    m_dirty = false;
}


