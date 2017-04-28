#pragma once

#include <glm/glm.hpp>



namespace deliberation
{

class Sphere;

class Ray3D final
{
public:
    static Ray3D fromTo(const glm::vec3 & from, const glm::vec3 & to);

public:
    Ray3D(const glm::vec3 & origin, const glm::vec3 & direction);

    const glm::vec3 & origin() const;
    const glm::vec3 & direction() const;

    void setOrigin(const glm::vec3& origin);
    void setDirection(const glm::vec3& direction);

//    bool intersects(const Sphere & sphere) const;

    Ray3D normalized() const;
    void normalize();

    glm::vec3 at(float s) const;

protected:
    glm::vec3 m_origin;
    glm::vec3 m_direction;
};

}

