#ifndef PLANE_H
#define PLANE_H

#include <iostream>
#include "objects/object.h"
#include "common.h"

class PlaneObject : public Object
{
public:
    PlaneObject(const Material *material, const Vec3 *point, const Vec3 &normal);

    RayObjIntersection rayIntersection(const Ray &ray) override;

    friend std::ostream &operator<<(std::ostream &ostr, const PlaneObject &plane);

private:
    const Vec3 *p;
    Vec3 n;
};

#endif
