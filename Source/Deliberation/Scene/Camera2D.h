#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

class Camera2D final
{
  public:
    Camera2D();
    Camera2D(float scale, const glm::vec2 & center, float aspectRatio);

    float             scale() const;
    const glm::vec2 & center() const;
    float             aspectRatio() const;

    void setScale(float scale);
    void setCenter(const glm::vec2 & center);
    void setAspectRatio(float aspectRatio);

    void update() const;

    // From ICamera
    virtual const glm::quat & orientation() const;
    virtual const glm::mat4 & view() const;
    virtual const glm::mat4 & projection() const;
    virtual const glm::mat4 & viewProjection() const;
    virtual const glm::mat4 & viewProjectionInverted() const;

  private:
    float     m_scale;
    glm::vec2 m_center;
    float     m_aspectRatio;
    glm::quat m_orientation;

    mutable bool      m_dirty;
    mutable glm::mat4 m_view;
    mutable glm::mat4 m_projection;
    mutable glm::mat4 m_viewProjection;
    mutable glm::mat4 m_viewProjectionInverted;
};
