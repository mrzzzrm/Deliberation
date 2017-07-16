#pragma once

#include <glm/glm.hpp>

namespace deliberation
{
class Plane final
{
public:
    Plane();
    Plane(const glm::vec3 & a, const glm::vec3 & b, const glm::vec3 & c);

    const glm::vec3 & normal() const;
    float             d() const;

    void setNormal(const glm::vec3 & normal);
    void setD(float d);

private:
    glm::vec3 m_normal;
    float     m_d;
};
}
