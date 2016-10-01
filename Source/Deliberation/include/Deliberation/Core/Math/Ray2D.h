#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class Sphere;

class DELIBERATION_API Ray2D final
{
public:
    static Ray2D fromTo(const glm::vec2 & from, const glm::vec2 & to);

public:
    Ray2D(const glm::vec2 & origin, const glm::vec2 & direction);

    const glm::vec2 & origin() const;
    const glm::vec2 & direction() const;

    void setOrigin(const glm::vec2& origin);
    void setDirection(const glm::vec2& direction);

    Ray2D normalized() const;
    void normalize();

    glm::vec2 at(float s) const;

protected:
    glm::vec2 m_origin;
    glm::vec2 m_direction;
};

}

