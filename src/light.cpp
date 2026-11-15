#include "light.h"

Light Light::createPoint(Vec3 intensity, const Vec3 &pos)
{
    return Light(LightType::Point, intensity, pos);
}

Light Light::createAmbient(Vec3 intensity)
{
    return Light(LightType::Ambient, intensity);
}

Light::Light(LightType type, Vec3 intensity) : type(type), intensity(intensity) {}

Light::Light(LightType type, Vec3 intensity, Vec3 pos) : type(type), intensity(intensity), pos(pos) {}

std::ostream &operator<<(std::ostream &ostr, const Light &light) {
    switch (light.type) {
        case LightType::Ambient:
            ostr << "AmbientLight(" << light.intensity << ")\n";
            break;
        case LightType::Point:
            ostr << "PointLight(" << light.pos << ", " << light.intensity << ")\n";
            break;
    }

    return ostr;
}
