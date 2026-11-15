#ifndef CYLINDER_H
#define CYLINDER_H

#include "objects/object.h"
#include "vec.h"

class CylinderObject : public Object {
public:
    CylinderObject(const Material *material, const Vec3 *center, const Vec3 &axis, float height, float radius);

    RayObjIntersection rayIntersection(const Ray &ray) override;

private:
    const Vec3 *c;
    const Vec3 a;
    float height, rad;
};

#endif
