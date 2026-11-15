#ifndef SCENE_H
#define SCENE_H

#include "vec.h"
#include "ray.h"
#include "material.h"
#include "camera.h"
#include "light.h"
#include "intersection.h"
#include "objects/object.h"
#include <fstream>
#include <vector>

class Scene
{
public:
    Vec3 backgroundColor;

    float shadowRayEpsilon = 1e-3;
    float maxRecursionDepth = 6;

    static Scene &parseSceneJson(std::ifstream &jsonFile);

    void addMaterial(Material material);

    const Material &getMaterial(int id) const;
    const Material *getMaterialP(int id) const;

    const Vec3 &getVertex(int id) const;
    const Vec3 *getVertexP(int id) const;

    const std::vector<Camera> &getCameras() const;
    const Light &getAmbientLight() const;
    const std::vector<Light> &getPointLights() const;

    RayObjIntersection intersectClosestObj(const Ray &pxRay) const;
    RayObjIntersection intersectAnyObj(const Ray &ray) const;

    ~Scene();

private:
    Scene() {};
    std::vector<Object *> objects;
    std::vector<Camera> cameras;
    std::vector<Light> pointLights;
    Light ambientLight = Light::createAmbient(Vec3(0.0));
    std::vector<Material> materials;
    std::vector<Vec3> verts;
};

#endif
