#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace deliberation
{
class Pose3D;

class Transform3D final
{
  public:
    static Transform3D         atPosition(const glm::vec3 & position);
    static Transform3D         atOrientation(const glm::quat & orientation);
    static Transform3D         withScale(float sclae);
    static const Transform3D & identity();
    static Transform3D         fromPose(const Pose3D & pose);

  public:
    Transform3D();

    const glm::vec3 & position() const;
    const glm::quat & orientation() const;
    const glm::vec3 & center() const;
    float             scale() const;

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setCenter(const glm::vec3 & center);
    void setScale(float scale);

    void setCenterAndAdjustPosition(const glm::vec3 & center);

    void localTranslate(const glm::vec3 & delta);
    void worldTranslate(const glm::vec3 & delta);

    void localRotate(const glm::quat & delta);
    void worldRotate(const glm::quat & delta);

    const glm::mat3 & basis() const;
    const glm::mat4 & matrix() const;

    glm::vec3 pointWorldToLocal(const glm::vec3 & point) const;
    glm::vec3 pointLocalToWorld(const glm::vec3 & point) const;
    glm::vec3 directionWorldToLocal(const glm::vec3 & dir) const;
    glm::vec3 directionLocalToWorld(const glm::vec3 & dir) const;

    Transform3D scaled(float scale) const;
    Transform3D worldTranslated(const glm::vec3 & delta) const;
    Transform3D interpolated(const Transform3D & other, float v) const;

    bool operator==(const Transform3D & other) const;
    bool operator!=(const Transform3D & other) const;

  protected:
    glm::vec3 m_position;
    glm::quat m_orientation;
    glm::vec3 m_center;
    float     m_scale;

    mutable glm::mat3 m_basis;
    mutable bool      m_basisDirty;

    mutable bool      m_matrixDirty;
    mutable glm::mat4 m_matrix;
};
}
