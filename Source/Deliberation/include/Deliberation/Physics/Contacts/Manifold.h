#pragma once

#include <string>

#include <glm/glm.hpp>

#include <Deliberation/Deliberation_API.h>

namespace deliberation
{

class DELIBERATION_API Manifold final
{
public:
    Manifold();
    Manifold(float depth,
             const glm::vec3 & position,
             const glm::vec3 & localPositionA,
             const glm::vec3 & localPositionB,
             const glm::vec3 & normal);

    float depth() const;
    const glm::vec3 & position() const;
    const glm::vec3 & localPositionA() const;
    const glm::vec3 & localPositionB() const;
    const glm::vec3 & normal() const;

    void setDepth(float depth);
    void setPosition(const glm::vec3 & position);
    void setLocalPointA(const glm::vec3 & localPositionA);
    void setLocalPointB(const glm::vec3 & localPositionB);
    void setNormal(const glm::vec3 & normal);

    std::string toString() const;

private:
    float     m_depth = 0.0f;
    glm::vec3 m_position;
    glm::vec3 m_localPositionA;
    glm::vec3 m_localPositionB;
    glm::vec3 m_normal;
};

}