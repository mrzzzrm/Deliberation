#pragma once

#include <glm/glm.hpp>

class Ray;
class Rect3D;
class Sphere;

bool NormalizedRaySphereIntersection(const glm::vec3 & origin,
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

bool PlaneRayIntersection(const glm::vec3 & normal,
                          float d,
                          const glm::vec3 & origin,
                          const glm::vec3 & delta,
                          float & t);

glm::vec3 PlaneRayIntersectionPoint(const glm::vec3 & normal,
                                    float d,
                                    const glm::vec3 & origin,
                                    const glm::vec3 & delta,
                                    bool & valid);

bool Rect3DRayIntersection(const glm::vec3 & base,
                           const glm::vec3 & right,
                           const glm::vec3 & up,
                           const glm::vec3 & origin,
                           const glm::vec3 & dir,
                           float & t);

bool Rect3DRayIntersection(const Rect3D & rect,
                           const Ray & ray,
                           float & t);

template<typename T1, typename T2>
bool Intersect(const T1 & p1, const T2 & p2);

template<>
bool Intersect(const Ray & ray, const Sphere & sphere);
