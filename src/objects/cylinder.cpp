#include "cylinder.h"

#include <algorithm>
#include <cmath>

CylinderObject::CylinderObject(const Material *material, const Vec3 *center, const Vec3 &axis, float height, float radius)
    : Object(material), c(center), a(axis.normalized()), height(height), rad(radius) {}

RayObjIntersection CylinderObject::rayIntersection(const Ray &ray) {
    float dDota = ray.d.dot(a);
    Vec3 dOrthogonal = ray.d - dDota * a;

    Vec3 deltaP = ray.o - *c;
    float dpDota = deltaP.dot(a);
    Vec3 dpOrthogonal = deltaP - a * dpDota;

    float A = dOrthogonal.length_squared();
    float B = 2 * dOrthogonal.dot(dpOrthogonal);
    float C = dpOrthogonal.dot(dpOrthogonal) - rad * rad;

    float discr = B * B - 4 * A * C;

    if (discr < 0) {
        return RayObjIntersection::noIntersection(ray);
    }

    float t0 = (-B - std::sqrt(discr)) / (2 * A);
    float t1 = (-B + std::sqrt(discr)) / (2 * A);

    if (t0 > t1) std::swap(t0, t1);

    float tHit = -1;
    Vec3 normalHit;

    float h0 = dpDota + t0 * dDota;
    if (t0 > 0.001 && h0 >= -height / 2.0 && h0 <= height / 2.0) {
        tHit = t0;
        Vec3 hitPoint = ray.at(tHit);
        normalHit = (hitPoint - (*c + a * h0)).normalized();
    } else {
        float h1 = dpDota + t1 * dDota;
        if (t1 > 0.001 && h1 >= -height / 2.0 && h1 <= height / 2.0) {
            tHit = t1;
            Vec3 hitPoint = ray.at(tHit);
            normalHit = (hitPoint - ((*c) + a * h1)).normalized();
        }
    }

    if (std::abs(dDota) > 0.00000001) {
        Vec3 topCenter = *c + (a) * (height / 2.0);
        float tTop = (topCenter - ray.o).dot(a) / dDota;
        if (tTop > 0.001 && (tHit < 0 || tTop < tHit)) {
            Vec3 pTop = ray.at(tTop);
            if ((pTop - topCenter).dot(pTop - topCenter) <= rad * rad) {
                tHit = tTop;
                normalHit = a;
            }
        }

        Vec3 botCenter = *c - (a) * (height / 2.0);
        float tBot = (botCenter - ray.o).dot(a) / dDota;
        if (tBot > 0.001 && (tHit < 0 || tBot < tHit)) {
            Vec3 pBot = ray.at(tBot);
            if ((pBot - botCenter).dot(pBot - botCenter) <= rad * rad) {
                tHit = tBot;
                normalHit = a * -1;
            }
        }
    }

    if (tHit > 0) {
        return RayObjIntersection(tHit, ray, normalHit, this);
    }

    return RayObjIntersection::noIntersection(ray);
}
