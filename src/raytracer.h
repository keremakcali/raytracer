#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <fstream>
#include "scene.h"

class RayTracer
{
public:
    RayTracer(const Scene &scene);

    void render();

private:
    RayObjIntersection traceRay(Ray ray, int depth) const;
    void writeColor(std::ofstream &file, Vec3 &color) const;
    Vec3 shade(const RayObjIntersection &intersection, int depth) const;
    Vec3 diffuseShading(const Light &light, const RayObjIntersection &objIntersection, const RayObjIntersection &shadowRayIntersection) const;
    Vec3 specularShading(const Light &light, const RayObjIntersection &objIntersection, const RayObjIntersection &shadowRay) const;
    Scene scene;
};

#endif
