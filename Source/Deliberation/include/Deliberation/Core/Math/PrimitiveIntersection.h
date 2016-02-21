#pragma once

#include <glm/glm.hpp>

namespace deliberation
{

class Ray3D;
class Rect3D;
class Sphere;

bool NormalizedRay3DSphereIntersection(const glm::vec3 & origin,
                                       const glm::vec3 & delta,
                                       const glm::vec3 & center,
                                       float radius);

bool LinearSphereSweepSphereIntersection(const glm::vec3 & centerA,
                                         float radiusA,
                                         const glm::vec3 & delta,
                                         const glm::vec3 & centerB,
                                         float radius);


bool HalfspaceContainsSphere(const glm::vec3 & normal,
                             float d,
                             const glm::vec3 & center,
                             float radius);

bool PlaneRay3DIntersection(const glm::vec3 & normal,
                            float d,
                            const glm::vec3 & origin,
                            const glm::vec3 & delta,
                            float & t);

glm::vec3 PlaneRay3DIntersectionPoint(const glm::vec3 & normal,
                                      float d,
                                      const glm::vec3 & origin,
                                      const glm::vec3 & delta,
                                      bool & valid);

bool Rect3DRay3DIntersection(const glm::vec3 & base,
                             const glm::vec3 & right,
                             const glm::vec3 & up,
                             const glm::vec3 & origin,
                             const glm::vec3 & dir,
                             float & t);

bool Rect3DRay3DIntersection(const Rect3D & rect,
                           const Ray3D & ray,
                           float & t);

template<typename T1, typename T2>
bool Intersect(const T1 & p1, const T2 & p2);

template<>
bool Intersect(const Ray3D & ray, const Sphere & sphere);

}

