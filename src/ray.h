#ifndef RAY_H
#define RAY_H

#include "vec.h"

class Ray {
public:
    Ray(const Vec3 &origin, const Vec3 &direction);

    Vec3 at(float t) const;
    Ray proj(const Vec3 &n, const Vec3 &p);

    Vec3 o, d;
};

#endif
