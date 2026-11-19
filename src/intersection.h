#ifndef INTERSECTION_H
#define INTERSECTION_H

#include "common.h"

class Object;

class RayObjIntersection
{
public:
    RayObjIntersection(float t, const Ray &ray, const Vec3 &normal, Object *obj);
    static RayObjIntersection noIntersection(Ray ray);

    float t;
    Ray ray;
    Vec3 normal;
    Object *obj;
};

#endif
