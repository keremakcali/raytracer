#ifndef OBJECT_H
#define OBJECT_H

#include "intersection.h"
#include "material.h"
#include "ray.h"

class Object
{
public:
    virtual ~Object() = default;
    virtual RayObjIntersection rayIntersection(const Ray &ray) = 0;

    const Material &getMaterial();

protected:
    Object(const Material *material);
    const Material *material;
};

#endif
