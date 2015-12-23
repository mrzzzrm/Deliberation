#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Math/Plane.h>

namespace deliberation
{

class Rect3D final
{
public:
    Rect3D(const glm::vec3 & origin,
           const glm::vec3 & right,
           const glm::vec3 & up);

    const glm::vec3 & origin() const;
    const glm::vec3 & right() const;
    const glm::vec3 & up() const;

    void setOrigin(const glm::vec3 & origin);
    void setRight(const glm::vec3 & right);
    void setUp(const glm::vec3 & up);

    Plane plane() const;

private:
    glm::vec3 m_origin;
    glm::vec3 m_right;
    glm::vec3 m_up;
};

}

