#pragma once

#include <string>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace deliberation
{

class Transform3D final
{
public:
    static Transform3D atPosition(const glm::vec3 & position);

public:
    Transform3D();

    const glm::vec3 & position() const;
    const glm::quat & orientation() const;
    const glm::vec3 & center() const;
    float scale() const;

    void setPosition(const glm::vec3 & position);
    void setOrientation(const glm::quat & orientation);
    void setCenter(const glm::vec3 & center);
    void setScale(float scale);

    void setCenterAndAdjustPosition(const glm::vec3 & center);

    void localTranslate(const glm::vec3 & delta);
    void worldTranslate(const glm::vec3 & delta);

    void localRotate(const glm::quat & delta);
    void worldRotate(const glm::quat & delta);

    const glm::mat4 & matrix() const;

    glm::vec3 pointWorldToLocal(const glm::vec3 & point) const;
    glm::vec3 pointLocalToWorld(const glm::vec3 & point) const;
    glm::vec3 directionWorldToLocal(const glm::vec3 & dir) const;
    glm::vec3 directionLocalToWorld(const glm::vec3 & dir) const;

    Transform3D interpolated(const Transform3D & other, float v);

    std::string toString() const;

    bool operator==(const Transform3D & other) const;
    bool operator!=(const Transform3D & other) const;

protected:
    glm::vec3           m_position;
    glm::quat           m_orientation;
    glm::vec3           m_center;
    float               m_scale;
    mutable bool        m_dirty;
    mutable glm::mat4   m_matrix;
};

}

