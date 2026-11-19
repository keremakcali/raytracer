#ifndef LIGHT_H
#define LIGHT_H

#include <iostream>
#include "common.h"

enum class LightType : int
{
    Point,
    Ambient
};

class Light
{
public:
    static Light createPoint(Vec3 intensity, const Vec3 &pos);
    static Light createAmbient(Vec3 intensity);

    friend std::ostream &operator<<(std::ostream &ostr, const Light &light);

    Light(LightType type, Vec3 intensity);
    Light(LightType type, Vec3 intensity, Vec3 pos);

    LightType type;
    Vec3 intensity = {200, 200, 200};
    Vec3 pos = {0.0, 0.0, 0.0};
};

#endif
