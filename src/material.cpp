#include "material.h"

Material::Material(Vec3 ambientR, Vec3 diffuseR, Vec3 specularR, Vec3 mirrorR, float phongExp)
    : ambientR(ambientR), diffuseR(diffuseR), specularR(specularR), mirrorR(mirrorR), phongExp(phongExp) {}

std::ostream &operator<<(std::ostream &ostr, Material m) {
    return ostr << "Material(" << "\n\t" << m.ambientR << "\n\t" << m.diffuseR << "\n\t"
                << m.specularR << "\n\t" << m.mirrorR << "\n\t" << m.phongExp << ")";
}
