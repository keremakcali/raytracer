#include "triangle.h"

TriangleObject::TriangleObject(const Material *material, const Vec3 *a, const Vec3 *b, const Vec3 *c)
    : Object(material), a(a), b(b), c(c)
{
    normal = (*b - *a).cross(*c - *a).normalized();
}

RayObjIntersection TriangleObject::rayIntersection(const Ray &ray)
{
    Vec3 n = normal;
    float dn = n.dot(ray.d);
    if (dn == 0)
    {
        return RayObjIntersection::noIntersection(ray);
    }
    float t = (*a - ray.o).dot(n) / dn;
    if (t < 0) {
        return RayObjIntersection::noIntersection(ray);
    }
    Vec3 p = ray.at(t);
    float s1 = (p - *a).cross(*b - *a).dot(-normal);
    if (s1 < 0) return RayObjIntersection::noIntersection(ray);
    float s2 = (p - *b).cross(*c - *b).dot(-normal);
    if (s2 < 0) return RayObjIntersection::noIntersection(ray);
    float s3 = (p - *c).cross(*a - *c).dot(-normal);
    if (s3 < 0) return RayObjIntersection::noIntersection(ray);

    return RayObjIntersection{t, ray, normal, this};
}
