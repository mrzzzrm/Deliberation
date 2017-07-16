#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace deliberation
{
class Transform3D;

class Pose3D final
{
public:
    inline static Pose3D         atPosition(const glm::vec3 & position);
    inline static Pose3D         atOrientation(const glm::quat & orientation);
    inline static const Pose3D & identity();

    static Pose3D fromTransform(const Transform3D & transform);
    static Pose3D
    fromTransformedPose(const Transform3D & transform, const Pose3D & pose);

public:
    Pose3D();
    Pose3D(
        const glm::vec3 & position,
        const glm::quat & orientation,
        const glm::vec3 & center = {});

    const glm::vec3 & position() const;
    const glm::quat & orientation() const;
    const glm::vec3 & center() const;

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setCenter(const glm::vec3 & center);

    void   localTranslate(const glm::vec3 & delta);
    Pose3D localTranslated(const glm::vec3 & delta) const;
    void   worldTranslate(const glm::vec3 & delta);
    Pose3D worldTranslated(const glm::vec3 & delta) const;

    void   localRotate(const glm::quat & delta);
    Pose3D localRotated(const glm::quat & delta) const;
    void   worldRotate(const glm::quat & delta);
    Pose3D worldRotated(const glm::quat & delta) const;

    const glm::mat3 & basis() const;
    const glm::mat4 & matrix() const;

    glm::vec3 pointWorldToLocal(const glm::vec3 & point) const;
    glm::vec3 pointLocalToWorld(const glm::vec3 & point) const;
    glm::vec3 directionWorldToLocal(const glm::vec3 & dir) const;
    glm::vec3 directionLocalToWorld(const glm::vec3 & dir) const;
    Pose3D    poseLocalToWorld(const Pose3D & local) const;

    Pose3D interpolated(const Pose3D & other, float v) const;

    bool operator==(const Pose3D & other) const;
    bool operator!=(const Pose3D & other) const;

protected:
    glm::vec3 m_position;
    glm::quat m_orientation;
    glm::vec3 m_center;

    mutable glm::mat3 m_basis;
    mutable bool      m_basisDirty;

    mutable bool      m_matrixDirty;
    mutable glm::mat4 m_matrix;
};
}

#include <Deliberation/Core/Math/Pose3D.inl>