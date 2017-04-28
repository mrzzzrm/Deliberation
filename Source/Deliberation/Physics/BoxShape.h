#pragma once

#include <glm/glm.hpp>

#include <Deliberation/Core/Math/Box.h>

#include <Deliberation/Physics/CollisionShape.h>



namespace deliberation
{

class BoxShape final:
    public CollisionShape
{
public:
    BoxShape(const glm::vec3 & halfExtent = glm::vec3());

    const glm::vec3 & halfExtent();

    void setHalfExtent(const glm::vec3 & halfExtent);

    Box instanciate(const Transform3D & transform) const;

    // From CollisionShape
    virtual AABB bounds(const Transform3D & transform) const override;
    virtual glm::mat3 localInertia() const override;

private:
    glm::vec3 m_halfExtent;
};

}