#ifndef OBJECT_H
#define OBJECT_H

#include "common.h"
#include "intersection.h"
#include "material.h"

class Object
{
public:
    virtual ~Object() = default;
    virtual RayObjIntersection rayIntersection(const Ray &ray) const = 0;

    const Material &getMaterial() const;

protected:
    Object(const Material *material);
    const Material *material;
};

#endif
