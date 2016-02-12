#include <Deliberation/Core/Math/PrimitiveIntersection.h>

#include <cassert>
#include <cmath>

#include <Deliberation/Core/Math/Plane.h>
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

bool PlaneRay3DIntersection(const glm::vec3 & normal,
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

glm::vec3 PlaneRay3DIntersectionPoint(const glm::vec3 & normal,
                                      float d,
                                      const glm::vec3 & origin,
                                      const glm::vec3 & delta,
                                      bool & valid)
{
    float t;
    if (!PlaneRay3DIntersection(normal, d, origin, delta, t))
    {
        valid = false;
        return {};
    }
    else
    {
        return origin + t * delta;
    }
}

bool Rect3DRay3DIntersection(const glm::vec3 & base,
                             const glm::vec3 & right,
                             const glm::vec3 & up,
                             const glm::vec3 & origin,
                             const glm::vec3 & dir,
                             float & t)
{
    auto plane = Rect3D(base, right, up).plane();
    auto valid = PlaneRay3DIntersection(plane.normal(), plane.d(),
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

bool Rect3DRay3DIntersection(const Rect3D & rect,
                             const Ray3D & ray,
                             float & t)
{
    return Rect3DRay3DIntersection(rect.origin(), rect.right(), rect.up(), ray.origin(), ray.direction(), t);
}

template<>
bool Intersect(const Ray3D & ray, const Sphere & sphere)
{
    auto nray = ray.normalized();
    return NormalizedRay3DSphereIntersection(nray.origin(), nray.direction(), sphere.position(), sphere.radius());
}

}

