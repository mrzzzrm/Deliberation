#include <Deliberation/Core/Math/PrimitiveIntersection.h>

#include <cassert>
#include <cmath>
#include <iostream>

#include <Deliberation/Core/Math/Plane.h>
#include <Deliberation/Core/Math/Ray2D.h>
#include <Deliberation/Core/Math/Ray3D.h>
#include <Deliberation/Core/Math/Rect3D.h>
#include <Deliberation/Core/Math/Sphere.h>

namespace deliberation
{

bool NormalizedRay3DSphereIntersection(const glm::vec3 & origin,
                                       const glm::vec3 & delta,
                                       const glm::vec3 & center,
                                       float radius)
{
    assert(std::abs(glm::length(delta) - 1.0f) < 0.001f); // Whatever

    auto m = origin - center;

    auto c = glm::dot(m, m) - radius * radius;
    if (c <= 0.0f)
    {
        return true;
    }

    auto b = glm::dot(m, delta);
    if (b > 0.0f) // Origin outside and pointing away
    {
        return false;
    }

    return b * b - c >= 0;
}

bool LineSphereIntersection(const glm::vec3 & origin,
                                             const glm::vec3 & delta,
                                             const glm::vec3 & center,
                                             float radius)
{
    auto tOrigin = origin - center;

    auto a = glm::dot(delta, delta);
    auto b = 2 * glm::dot(tOrigin, delta);
    auto c = glm::dot(tOrigin, tOrigin) - radius * radius;

    auto underSqrt = b * b - 4 * a * c;

    // We're starting inside the Sphere
    if (c <= 0)
    {
        return true;
    }

    if (underSqrt < 0)
    {
        return false;
    }

    auto s1 = (-b + std::sqrt(underSqrt)) / (2 * a);
    auto s2 = (-b - std::sqrt(underSqrt)) / (2 * a);

    return (s1 >= 0 && s1 <= 1) || (s2 >= 0 && s2 <= 1);

    std::cout << s1 << " " << s2 << std::endl;

    if (b >= 0)
    {
        auto greaterZero = c <= 0 || a <= 0;
        auto smallerOne = 2 * a >= -b && a + b >= -c;
        return greaterZero && smallerOne;
    }
    else
    {
        auto s1 = 2 * a >= -b && a + b >= -c;
        auto s2 = (a == 0 || c >= 0) && (2 * a >= -b || a + b <= -c);
        return s1 || s2;
    }
}

bool LinearSphereSweepSphereIntersection(const glm::vec3 & centerA,
                                         float radiusA,
                                         const glm::vec3 & delta,
                                         const glm::vec3 & centerB,
                                         float radiusB)
{
    //auto radiusC = radiusA + radiusB;
    return true;//RaySphereIntersection(centerA, delta, centerB, radiusB);
}

bool HalfspaceContainsSphere(const glm::vec3 & normal,
                             float d,
                             const glm::vec3 & center,
                             float radius)
{
    auto distance = glm::dot(center, normal) - d;
    return distance > radius;
}

bool Ray3DPlaneIntersection(const glm::vec3 & normal,
                            float d,
                            const glm::vec3 & origin,
                            const glm::vec3 & delta,
                            float & t)
{
    auto l = glm::dot(delta, normal);
    if (l == 0.0f) // TODO: some epsilon that floats can handle being divided by
    {
        t = 0.0f;
        return true;
    }
    else
    {
        t = (d - glm::dot(origin, normal)) / l;
        return t >= 0.0f;
    }
}

glm::vec3 Ray3DPlaneIntersection(const glm::vec3 & normal,
                                 float d,
                                 const glm::vec3 & origin,
                                 const glm::vec3 & delta,
                                 bool & valid)
{
    float t;
    if (!Ray3DPlaneIntersection(normal, d, origin, delta, t))
    {
        valid = false;
        return {};
    }
    else
    {
        return origin + t * delta;
    }
}

glm::vec2 Rect3DRay3DIntersectionPoint(const glm::vec3 & base,
                                                        const glm::vec3 & right,
                                                        const glm::vec3 & up,
                                                        const glm::vec3 & origin,
                                                        const glm::vec3 & delta,
                                                        bool & valid)
{
    auto plane = Rect3D(base, right, up).plane();
    float t;
    valid = Ray3DPlaneIntersection(plane.normal(), plane.d(),
                                        origin, delta, t);
    if (!valid)
    {
        return {};
    }

    auto point = origin + t * delta;
    point -= base;

    // Project point on right and up
    // Projection on r=pr, Projection on u=pu

    auto pr = glm::dot(point, right) / glm::dot(right, right);
    if (pr < 0.0f || pr > 1.0f)
    {
        valid = false;
        return {};
    }

    auto pu = glm::dot(point, up) / glm::dot(up, up);
    if (pu < 0.0f || pu > 1.0f)
    {
        valid = false;
        return {};
    }

    valid = true;
    return {pr, pu};
}

glm::vec2 Rect3DRay3DIntersectionPoint(const Rect3D & rect,
                                       const Ray3D & ray,
                                       bool & valid)
{
    return Rect3DRay3DIntersectionPoint(rect.origin(), rect.right(), rect.up(), ray.origin(), ray.direction(), valid);
}


bool Ray3DRect3DIntersection(const glm::vec3 & base,
                             const glm::vec3 & right,
                             const glm::vec3 & up,
                             const glm::vec3 & origin,
                             const glm::vec3 & dir,
                             float & t)
{
    auto plane = Rect3D(base, right, up).plane();
    auto valid = Ray3DPlaneIntersection(plane.normal(), plane.d(),
                                      origin, dir, t);
    if (!valid)
    {
        return false;
    }

    auto point = origin + t * dir;
    point -= base;

    // Project point on right and up
    // Projection on r=pr, Projection on u=pu

    auto pr = glm::dot(point, right) / glm::dot(right, right);
    if (pr < 0.0f || pr > 1.0f)
    {
        return false;
    }

    auto pu = glm::dot(point, up) / glm::dot(up, up);
    if (pu < 0.0f || pu > 1.0f)
    {
        return false;
    }

    return true;
}

void Ray3DClosestApproach(const glm::vec3 & oA,
                          const glm::vec3 & dA,
                          const glm::vec3 & oB,
                          const glm::vec3 & dB,
                          float & a,
                          float & b)
{
    glm::vec3 p = oB - oA;
    auto uaub = glm::dot(dA, dB);
    auto q1 =  glm::dot(dA, p);
    auto q2 = -glm::dot(dB, p);
    auto d = 1-uaub*uaub;

    if (d <= 0.0001f)
    {
        a = 0;
        b = 0;
    }
    else
    {
        d = 1.0f / d;
        a = (q1 + uaub*q2)*d;
        b  = (uaub*q1 + q2)*d;
    }
}


bool Ray3DRect3DIntersection(const Rect3D & rect,
                         const Ray3D & ray,
                         float & t)
{
    return Ray3DRect3DIntersection(rect.origin(), rect.right(), rect.up(), ray.origin(), ray.direction(), t);
}

float PointRay2DHalfspace(const glm::vec2 & point,
                          const glm::vec2 & origin,
                          const glm::vec2 & direction)
{
    glm::vec2 normal(direction.y, -direction.x);
    return glm::dot(normal, point - origin);
}

float PointRay2DHalfspace(const glm::vec2 & point,
                          const Ray2D & ray)
{
    return PointRay2DHalfspace(point, ray.origin(), ray.direction());
}

glm::vec2 Ray2DIntersectionPoint(const glm::vec2 & oA,
                                 const glm::vec2 & dA,
                                 const glm::vec2 & oB,
                                 const glm::vec2 & dB,
                                 bool & exists)
{
    auto w = oA - oB;
    auto d = (dA.x * dB.y - dB.x * dA.y);

    if (d == 0.0f)
    {
        exists = false;
        return {};
    }

    exists = true;

    auto n = (dB.x * w.y - dB.y * w.x);
    auto s = n / d;
    auto r = oA + s * dA;

    return r;
}

glm::vec2 Ray2DIntersectionPoint(const Ray2D & a, const Ray2D & b, bool & exists)
{
    return Ray2DIntersectionPoint(a.origin(), a.direction(), b.origin(), b.direction(), exists);
}

template<>
bool Intersect(const Ray3D & ray, const Sphere & sphere)
{
    auto nray = ray.normalized();
    return NormalizedRay3DSphereIntersection(nray.origin(), nray.direction(), sphere.position(), sphere.radius());
}

}

