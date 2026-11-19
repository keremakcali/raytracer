#include "intersection.h"

#include <limits>

RayObjIntersection::RayObjIntersection(float t, const Ray &ray, const Vec3 &normal, const Object *obj)
    : t(t), ray(ray), normal(normal), obj(obj) {}

RayObjIntersection RayObjIntersection::noIntersection(Ray ray) {
    return RayObjIntersection{std::numeric_limits<float>::max(), ray, Vec3(0.0), nullptr};
}
