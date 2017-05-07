#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Deliberation/Core/Math/Pose3D.h>
#include <Deliberation/Core/Math/Rect3D.h>

namespace deliberation
{
class Camera3D final
{
  public:
    Camera3D();

    const glm::vec3 & position() const;
    const glm::quat & orientation() const;
    float             zNear() const;
    float             zFar() const;
    float             yFoV() const;
    float             aspectRatio() const;
    const Pose3D &    pose() const;

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setZNear(float zNear);
    void setZFar(float zFar);
    void setYFoV(float yFoV);
    void setAspectRatio(float aspectRatio);

    const glm::mat4 & view() const;
    const glm::mat4 & projection() const;
    const glm::mat4 & viewProjection() const;

    Rect3D nearPlane() const;
    Rect3D farPlane() const;

    glm::vec2 projectToNearPlane(const glm::vec3 & point, bool & success) const;

    std::string toString() const;

  private:
    glm::vec2 zPlaneSize(float z) const;

  private:
    Pose3D            m_pose;
    float             m_zNear;
    float             m_zFar;
    float             m_yFoV;
    float             m_aspectRatio;
    mutable bool      m_viewDirty;
    mutable bool      m_projectionDirty;
    mutable glm::mat4 m_view;
    mutable glm::mat4 m_projection;
    mutable glm::mat4 m_viewProjection;
};
}
