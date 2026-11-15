#ifndef MATERIAL_H
#define MATERIAL_H

#include <iostream>
#include "vec.h"

class Material {
public:
    Material(Vec3 ambientR, Vec3 diffuseR, Vec3 specularR, Vec3 mirrorR, float phongExp);

    Vec3 ambientR, diffuseR, specularR, mirrorR;
    float phongExp;

    friend std::ostream &operator<<(std::ostream &ostr, Material m);
};

#endif
