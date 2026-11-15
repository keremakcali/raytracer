#include "ray.h"

#include <cmath>

Ray::Ray(const Vec3 &origin, const Vec3 &direction) : o(origin), d(direction) {
    if (std::abs(d.length() - 1) > 0.001) {
        throw 1;
    }
}

Vec3 Ray::at(float t) const {
    return o + t * d;
}

Ray Ray::proj(const Vec3 &n, const Vec3 &p) { // plane projection
    Vec3 o_proj = o - o.proj(n);
    Vec3 dir_proj = d - d.proj(n);
    return Ray(o - o.proj(n), d - d.proj(n));
}
