#include "sphere.h"

#include <cmath>

SphereObject::SphereObject(const Material *material, const Vec3 *center, float radius)
    : Object(material), center(center), rad(radius) {}

RayObjIntersection SphereObject::rayIntersection(const Ray &ray) {
    Vec3 C = *center;
    Vec3 o = ray.o;
    Vec3 d = ray.d;
    Vec3 oc = o - C;
    float b = 2 * d.dot(oc);
    float c = oc.length_squared() - rad * rad;
    float det = b * b - 4 * c;
    if (det < 0) {
        return RayObjIntersection::noIntersection(ray);
    }
    float sqrtDet = std::sqrt(det);
    float t = (-b - sqrtDet) / 2;
    if (t < 0) {
        t = (-b + sqrtDet) / 2;
    }
    if (t < 0) {
        return RayObjIntersection::noIntersection(ray);
    }
    Vec3 normal = (o + d * t - C).normalized();

    return RayObjIntersection{t, ray, normal, this};
}

std::ostream &operator<<(std::ostream &ostr, const SphereObject &sphere) {
    return ostr << "Sphere( " << *sphere.center << ", " << sphere.rad << ")";
}
