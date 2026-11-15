#ifndef SPHERE_H
#define SPHERE_H

#include <iostream>
#include "objects/object.h"
#include "vec.h"

class SphereObject : public Object {
public:
    SphereObject(const Material *material, const Vec3 *center, float radius);

    RayObjIntersection rayIntersection(const Ray &ray) override;

    friend std::ostream &operator<<(std::ostream &ostr, const SphereObject &sphere);

private:
    const Vec3 *center;
    float rad;
};

#endif
