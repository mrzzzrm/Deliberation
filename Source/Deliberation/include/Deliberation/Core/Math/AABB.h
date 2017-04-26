#pragma once

#include <vector>

#include <glm/glm.hpp>



namespace deliberation
{

class Sphere;

class AABB final
{
public:
    static AABB containingPoints(const std::vector<glm::vec3> & points);
    static AABB containingAABBs(const std::vector<AABB> & aabbs);
    static AABB containingSpheres(const std::vector<Sphere> & spheres);
    static AABB containingSphere(const Sphere & sphere);

public:
    AABB();
    AABB(const glm::vec3 & llf, const glm::vec3 & urb);

    const glm::vec3 & llf() const;
    const glm::vec3 & urb() const;

    glm::vec3 llb() const;
    glm::vec3 ulf() const;
    glm::vec3 ulb() const;
    glm::vec3 lrb() const;
    glm::vec3 urf() const;
    glm::vec3 lrf() const;

    float left() const;
    float right() const;
    float bottom() const;
    float top() const;
    float front() const;
    float back() const;

    void setLLF(const glm::vec3 & llf);
    void setURB(const glm::vec3 & urb);

    void setLeft(float left);
    void setRight(float right);
    void setBottom(float bottom);
    void setTop(float top);
    void setFront(float front);
    void setBack(float back);

    unsigned int longestAxis() const;

    glm::vec3 center() const;
    glm::vec3 size() const;
    float diameter() const;

    void enlargeToContain(const glm::vec3 & v);
    void enlargeToContain(const AABB & other);
    void enlargeToContain(const Sphere & sphere);
    void enlarge(float margin);

    AABB enlargedToContain(const glm::vec3 & v) const;
    AABB enlargedToContain(const AABB & other) const;
    AABB enlargedToContain(const Sphere & sphre) const;
    AABB enlargedByMargin(float margin) const;

    void translate(const glm::vec3 & delta);

    bool intersects(const AABB & other) const;
    bool contains(const AABB & other) const;

private:
    glm::vec3 m_llf;
    glm::vec3 m_urb;
};

}

