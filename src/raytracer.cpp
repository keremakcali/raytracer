#include "raytracer.h"

#include <cmath>
#include <iostream>
#include <vector>

RayTracer::RayTracer(const Scene &scene) : scene(scene) {}

Bitmap RayTracer::render() {
    if (scene.getCameras().empty()) {
        throw RayTracerException("No cameras available to render");
    }
    const Camera &cam = scene.getCameras()[0];
    const int res_h = cam.res_h;
    const int res_v = cam.res_v;

    int totalPixel = res_h * res_v;
    std::vector<Color> pixelVals(totalPixel);
    for (int y = 0; y < res_v; y++) {
        std::cout << "\r" << res_h * y << "/" << totalPixel << "           ";
        for (int x = 0; x < res_h; x++) {
            Ray pxRay = cam.getPxRay(x, y);
            RayObjIntersection rayIntersection = scene.intersectClosestObj(pxRay);
            Vec3 color = shade(rayIntersection, 0).round().clip(255);
            int nPix = (y * res_h) + x;
            pixelVals[nPix] = Color{
                static_cast<uint8_t>(color.r()),
                static_cast<uint8_t>(color.g()),
                static_cast<uint8_t>(color.b())
            };
        }
    }

    std::cout << "\r Completed!         \n";
    return Bitmap{res_h, res_v, pixelVals};
}

Vec3 RayTracer::shade(const RayObjIntersection &intersection, int depth) const {
    if (!intersection.obj) {
        return scene.backgroundColor;
    }
    if (depth > scene.maxRecursionDepth) {
        return Vec3(0.0);
    }

    Vec3 totalShade{0.0};

    Vec3 pos = intersection.ray.at(intersection.t);
    const std::vector<Light> &lights = scene.getPointLights();
    int nLights = lights.size();
    for (int i = 0; i < nLights; i++) {

        Light light = lights[i];
        Vec3 lightVec = light.pos - pos;
        if (lightVec.dot(intersection.normal) <= 0)
        {
            continue;
        }
        float lightT = lightVec.length();
        Ray shadowRay{pos, lightVec.normalized()};
        shadowRay.o += shadowRay.d * scene.shadowRayEpsilon;
        RayObjIntersection shadowRayIntersection = scene.intersectClosestObj(shadowRay);

        if (shadowRayIntersection.t < lightT)
        {
            continue;
        }

        totalShade += diffuseShading(light, intersection, shadowRayIntersection) + specularShading(light, intersection, shadowRayIntersection);
    }

    if (intersection.obj->getMaterial().mirrorR.length_squared() > 0 && depth < scene.maxRecursionDepth)
    {
        Vec3 intersectionPos = intersection.ray.at(intersection.t);
        Vec3 a = intersection.ray.d.proj(intersection.normal);
        Ray reflected{intersectionPos, (intersection.ray.d - 2 * a).normalized()};
        reflected.o += reflected.d * scene.shadowRayEpsilon;
        RayObjIntersection mirrorThing = scene.intersectClosestObj(reflected);
        Vec3 mirrorShade = shade(mirrorThing, depth + 1) * intersection.obj->getMaterial().mirrorR;
        totalShade += mirrorShade;
    }

        Vec3 ambientShade = scene.getAmbientLight().intensity * intersection.obj->getMaterial().ambientR;

        totalShade += ambientShade;
        return totalShade;
    }

Vec3 RayTracer::diffuseShading(const Light &light, const RayObjIntersection &objIntersection, const RayObjIntersection &shadowRayIntersection) const {

        float cosp = std::fmax(0, shadowRayIntersection.ray.d.dot(objIntersection.normal));
        Vec3 shade = objIntersection.obj->getMaterial().diffuseR * light.intensity * cosp / (light.pos-objIntersection.ray.at(objIntersection.t)).length_squared();
        return shade;
    }

Vec3 RayTracer::specularShading(const Light &light, const RayObjIntersection &objIntersection, const RayObjIntersection &shadowRayIntersection) const {
        Vec3 h = (shadowRayIntersection.ray.d - objIntersection.ray.d).normalized();
        float cosp = std::fmax(0, h.dot(objIntersection.normal));
        Vec3 shade = objIntersection.obj->getMaterial().specularR * light.intensity * pow(cosp, objIntersection.obj->getMaterial().phongExp) / (light.pos-objIntersection.ray.at(objIntersection.t)).length_squared();
        return shade;
    }