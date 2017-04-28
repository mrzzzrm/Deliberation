#pragma once

#include <glm/glm.hpp>



namespace deliberation
{

class AABB;

class Sphere final
{
public:
    static Sphere containingAABB(const AABB & aabb);

public:
    Sphere();
    Sphere(const glm::vec3 & position, float radius);

    float radius() const;
    const glm::vec3 & position() const;

    void setRadius(float radius);
    void setPosition(const glm::vec3 & position);

    bool intersects(const Sphere & other) const;

protected:
    glm::vec3   m_position;
    float       m_radius;
};

}

