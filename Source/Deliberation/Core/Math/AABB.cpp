#include <Deliberation/Core/Math/AABB.h>

#include <algorithm>
#include <cassert>
#include <iostream>

#include <Deliberation/Core/Assert.h>

#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{
AABB AABB::containingPoints(const std::vector<glm::vec3> & points)
{
    assert(!points.empty());

    AABB result(points[0], points[0]);
    for (auto p = 1u; p < points.size(); p++)
    {
        result.enlargeToContain(points[p]);
    }

    return result;
}

AABB AABB::containingAABBs(const std::vector<AABB> & aabbs)
{
    assert(!aabbs.empty());

    auto result = aabbs[0];
    for (auto b = 1u; b < aabbs.size(); b++)
    {
        result.enlargeToContain(aabbs[b]);
    }

    return result;
}

AABB AABB::containingSpheres(const std::vector<Sphere> & spheres)
{
    assert(!spheres.empty());

    auto result = AABB::containingSphere(spheres[0]);
    for (auto s = 1u; s < spheres.size(); s++)
    {
        result.enlargeToContain(spheres[s]);
    }

    return result;
}

AABB AABB::containingSphere(const Sphere & sphere)
{
    return AABB(
        {sphere.position() - glm::vec3(sphere.radius())},
        {sphere.position() + glm::vec3(sphere.radius())});
}

AABB::AABB() = default;

AABB::AABB(const glm::vec3 & llf, const glm::vec3 & urb)
    : m_llf(llf), m_urb(urb)
{
}

const glm::vec3 & AABB::llf() const { return m_llf; }

const glm::vec3 & AABB::urb() const { return m_urb; }

glm::vec3 AABB::llb() const { return {m_llf.x, m_llf.y, m_urb.z}; }

glm::vec3 AABB::ulf() const { return {m_llf.x, m_urb.y, m_llf.z}; }

glm::vec3 AABB::ulb() const { return {m_llf.x, m_urb.y, m_urb.z}; }

glm::vec3 AABB::lrb() const { return {m_urb.x, m_llf.y, m_urb.z}; }

glm::vec3 AABB::urf() const { return {m_urb.x, m_urb.y, m_llf.z}; }

glm::vec3 AABB::lrf() const { return {m_urb.x, m_llf.y, m_llf.z}; }

float AABB::left() const { return m_llf.x; }

float AABB::right() const { return m_urb.x; }

float AABB::bottom() const { return m_llf.y; }

float AABB::top() const { return m_urb.y; }

float AABB::front() const { return m_llf.z; }

float AABB::back() const { return m_urb.z; }

void AABB::setLLF(const glm::vec3 & llf) { m_llf = llf; }

void AABB::setURB(const glm::vec3 & urb) { m_urb = urb; }

void AABB::setLeft(float left)
{
    Assert(left <= right());
    m_llf.x = left;
}

void AABB::setRight(float right)
{
    Assert(right >= left());
    m_urb.x = right;
}

void AABB::setBottom(float bottom)
{
    Assert(bottom <= top());
    m_llf.y = bottom;
}

void AABB::setTop(float top)
{
    Assert(top >= bottom());
    m_urb.y = top;
}

void AABB::setFront(float front)
{
    Assert(front <= back());
    m_llf.z = front;
}

void AABB::setBack(float back)
{
    Assert(back >= front());
    m_urb.z = back;
}

unsigned int AABB::longestAxis() const
{
    auto size = this->size();
    return size.x > size.y ? size.x > size.z ? 0 : size.y > size.z ? 1 : 2
                           : size.y > size.z ? 1 : 2;
}

glm::vec3 AABB::center() const { return (m_urb + m_llf) / 2.0f; }

glm::vec3 AABB::size() const { return m_urb - m_llf; }

float AABB::diameter() const { return glm::length(m_urb - m_llf); }

void AABB::enlargeToContain(const glm::vec3 & v)
{
    m_llf.x = std::min(m_llf.x, v.x);
    m_llf.y = std::min(m_llf.y, v.y);
    m_llf.z = std::min(m_llf.z, v.z);

    m_urb.x = std::max(m_urb.x, v.x);
    m_urb.y = std::max(m_urb.y, v.y);
    m_urb.z = std::max(m_urb.z, v.z);
}

void AABB::enlargeToContain(const AABB & other)
{
    enlargeToContain(other.m_llf);
    enlargeToContain(other.m_urb);
}

void AABB::enlargeToContain(const Sphere & sphere)
{
    enlargeToContain(AABB::containingSphere(sphere));
}

void AABB::enlarge(float margin)
{
    m_llf -= glm::vec3(margin);
    m_urb += glm::vec3(margin);
}

AABB AABB::enlargedToContain(const glm::vec3 & v) const
{
    auto dummy = *this;
    dummy.enlargeToContain(v);
    return dummy;
}

AABB AABB::enlargedToContain(const AABB & other) const
{
    auto dummy = *this;
    dummy.enlargeToContain(other);
    return dummy;
}

AABB AABB::enlargedToContain(const Sphere & sphere) const
{
    auto dummy = *this;
    dummy.enlargeToContain(sphere);
    return dummy;
}

AABB AABB::enlargedByMargin(float margin) const
{
    auto dummy = *this;
    dummy.enlarge(margin);
    return dummy;
}

void AABB::translate(const glm::vec3 & delta)
{
    m_llf += delta;
    m_urb += delta;
}

bool AABB::intersects(const AABB & other) const
{
    if (m_llf.x < other.llf().x)
    {
        if (other.llf().x > m_urb.x)
        {
            return false;
        }
    }
    else
    {
        if (m_llf.x > other.urb().x)
        {
            return false;
        }
    }

    if (m_llf.y < other.llf().y)
    {
        if (other.llf().y > m_urb.y)
        {
            return false;
        }
    }
    else
    {
        if (m_llf.y > other.urb().y)
        {
            return false;
        }
    }

    if (m_llf.z < other.llf().z)
    {
        if (other.llf().z > m_urb.z)
        {
            return false;
        }
    }
    else
    {
        if (m_llf.z > other.urb().z)
        {
            return false;
        }
    }

    return true;
}

bool AABB::contains(const AABB & other) const
{
    return m_llf.x <= other.m_llf.x && m_llf.y <= other.m_llf.y &&
           m_llf.z <= other.m_llf.z && m_urb.x >= other.m_urb.x &&
           m_urb.y >= other.m_urb.y && m_urb.z >= other.m_urb.z;
}
}

std::ostream & operator<<(std::ostream & os, const deliberation::AABB & aabb)
{
    os << "{" << aabb.llf() << " -> " << aabb.urb() << "}";
    return os;
}