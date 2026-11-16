#include "plane.h"

PlaneObject::PlaneObject(const Material *material, const Vec3 *point, const Vec3 &normal)
    : Object(material), p(point), n(normal.normalized()) {}

RayObjIntersection PlaneObject::rayIntersection(const Ray &ray) {
    float dn = n.dot(ray.d);
    if (dn == 0) {
        return RayObjIntersection::noIntersection(ray);
    }
    float t = (*p - ray.o).dot(n) / dn;
    return RayObjIntersection{t, ray, n, this};
}

std::ostream &operator<<(std::ostream &ostr, const PlaneObject &plane) {
    return ostr << "Plane( " << *plane.p << ", " << plane.n << ")";
}
