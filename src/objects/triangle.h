#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "objects/object.h"
#include "common.h"

class TriangleObject : public Object
{
public:
    TriangleObject(const Material *material, const Vec3 *a, const Vec3 *b, const Vec3 *c);

    RayObjIntersection rayIntersection(const Ray &ray) override;

private:
    const Vec3 *a, *b, *c;
    Vec3 normal;
};

#endif
