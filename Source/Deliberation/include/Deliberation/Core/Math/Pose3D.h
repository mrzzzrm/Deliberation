#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace deliberation
{

class Pose3D final
{
public:
    inline static Pose3D atPosition(const glm::vec3 & position);
    inline static Pose3D atOrientation(const glm::quat & orientation);
    inline static const Pose3D & identity();

public:
    inline Pose3D();
    inline Pose3D(const glm::vec3 & position, glm::quat & orientation);

    const glm::vec3 & position() const;
    const glm::quat & orientation() const;

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);

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

    Pose3D worldTranslated(const glm::vec3 & delta) const;
    Pose3D interpolated(const Pose3D & other, float v) const;

    std::string toString() const;

    bool operator==(const Pose3D & other) const;
    bool operator!=(const Pose3D & other) const;

protected:
    glm::vec3           m_position;
    glm::quat           m_orientation;

    mutable glm::mat3   m_basis;
    mutable bool        m_basisDirty;

    mutable bool        m_matrixDirty;
    mutable glm::mat4   m_matrix;
};

}

#include <Deliberation/Core/Math/Pose3D.inl>