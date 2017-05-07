#pragma once

#include <glm/glm.hpp>

namespace deliberation
{
class Ray2D;
class Ray3D;
class Rect3D;
class Sphere;

bool NormalizedRay3DSphereIntersection(
    const glm::vec3 & origin,
    const glm::vec3 & delta,
    const glm::vec3 & center,
    float             radius);

bool LineSphereIntersection(
    const glm::vec3 & pointA,
    const glm::vec3 & pointB,
    const glm::vec3 & center,
    float             radius);

bool LinearSphereSweepSphereIntersection(
    const glm::vec3 & centerA,
    float             radiusA,
    const glm::vec3 & delta,
    const glm::vec3 & centerB,
    float             radius);

bool HalfspaceContainsSphere(
    const glm::vec3 & normal, float d, const glm::vec3 & center, float radius);

bool Ray3DPlaneIntersection(
    const glm::vec3 & normal,
    float             d,
    const glm::vec3 & origin,
    const glm::vec3 & delta,
    float &           t);

glm::vec3 Ray3DPlaneIntersectionPoint(
    const glm::vec3 & normal,
    float             d,
    const glm::vec3 & origin,
    const glm::vec3 & delta,
    bool &            valid);

glm::vec2 Rect3DRay3DIntersectionPoint(
    const glm::vec3 & base,
    const glm::vec3 & right,
    const glm::vec3 & up,
    const glm::vec3 & origin,
    const glm::vec3 & delta,
    bool &            valid);

glm::vec2 Rect3DRay3DIntersectionPoint(
    const Rect3D & rect, const Ray3D & ray, bool & valid);

bool Ray3DRect3DIntersection(
    const glm::vec3 & base,
    const glm::vec3 & right,
    const glm::vec3 & up,
    const glm::vec3 & origin,
    const glm::vec3 & dir,
    float &           t);

bool Ray3DRect3DIntersection(const Rect3D & rect, const Ray3D & ray, float & t);

void Ray3DClosestApproach(
    const glm::vec3 & oA,
    const glm::vec3 & dA,
    const glm::vec3 & oB,
    const glm::vec3 & dB,
    float &           a,
    float &           b);

float PointRay2DHalfspace(
    const glm::vec2 & point,
    const glm::vec2 & origin,
    const glm::vec2 & direction);

float PointRay2DHalfspace(const glm::vec2 & point, const Ray2D & ray);

glm::vec2 Ray2DIntersectionPoint(
    const glm::vec2 & originA,
    const glm::vec2 & directionA,
    const glm::vec2 & originB,
    const glm::vec2 & directionB,
    bool &            exists);

glm::vec2
Ray2DIntersectionPoint(const Ray2D & a, const Ray2D & b, bool & exists);

template<typename T1, typename T2>
bool Intersect(const T1 & p1, const T2 & p2);

template<>
bool Intersect(const Ray3D & ray, const Sphere & sphere);
}
