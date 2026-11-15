#include "scene.h"
#include "picojson.h"
#include "objects/sphere.h"
#include "objects/triangle.h"
#include "objects/plane.h"
#include "objects/cylinder.h"

#include <cmath>
#include <iostream>
#include <sstream>

const Material &Scene::getMaterial(int id) const {
    return materials[id - 1];
}

const Material *Scene::getMaterialP(int id) const {
    return &materials[id - 1];
}

const Vec3 &Scene::getVertex(int id) const {
    return verts[id - 1];
}

const Vec3 *Scene::getVertexP(int id) const {
    return &verts[id - 1];
}

const std::vector<Camera> &Scene::getCameras() const {
    return cameras;
}

const Light &Scene::getAmbientLight() const {
    return ambientLight;
}

const std::vector<Light> &Scene::getPointLights() const {
    return pointLights;
}

RayObjIntersection Scene::intersectClosestObj(const Ray &ray) const
{
    RayObjIntersection closest = RayObjIntersection::noIntersection(ray);
    int nObjects = objects.size();
    for (int i = 0; i < nObjects; i++)
    {
        Object *obj = objects[i];
        RayObjIntersection intersection = obj->rayIntersection(ray);
        if (intersection.t < 0) {
            continue;
        }

        if (intersection.t < closest.t) {
            closest = intersection;
        }
    }

    return closest;
}


Scene &Scene::parseSceneJson(std::ifstream &jsonFile)
{

    picojson::value v;
    std::string err = picojson::parse(v, jsonFile);

    Scene &scene = *(new Scene{});
    if (!err.empty())
    {
        std::cerr << "Could not parse the file as a json" << std::endl;
        return scene;
    }

    if (!v.is<picojson::object>())
    {
        std::cerr << "JSON is not an object" << std::endl;
        return scene;
    }

    auto getStr = [](const picojson::value &val) -> std::string {
        if (val.is<std::string>()) {
            return val.get<std::string>();
        }
        if (val.is<picojson::object>()) {
            const picojson::object &obj = val.get<picojson::object>();
            if (auto it = obj.find("_data"); it != obj.end() && it->second.is<std::string>()) {
                return it->second.get<std::string>();
            }
        }
        return val.to_str();
    };

    auto asArray = [](const picojson::value &val) -> picojson::array {
        if (val.is<picojson::array>()) {
            return val.get<picojson::array>();
        }
        if (val.is<picojson::null>()) {
            return picojson::array();
        }
        return picojson::array(1, val);
    };

    const picojson::object &rootObj = v.get<picojson::object>();
    const picojson::object &sceneJson = (rootObj.find("Scene") != rootObj.end() && rootObj.at("Scene").is<picojson::object>())
                                            ? rootObj.at("Scene").get<picojson::object>()
                                            : rootObj;

    if (auto field = sceneJson.find("BackgroundColor"); field != sceneJson.end()) {
        scene.backgroundColor = Vec3(getStr(field->second));
    }

    if (auto field = sceneJson.find("ShadowRayEpsilon"); field != sceneJson.end()) {
        scene.shadowRayEpsilon = std::stof(getStr(field->second));
    }

    if (auto field = sceneJson.find("MaxRecursionDepth"); field != sceneJson.end()) {
        scene.maxRecursionDepth = std::stof(getStr(field->second));
    }
    

    if (auto camsField = sceneJson.find("Cameras"); camsField != sceneJson.end() && camsField->second.is<picojson::object>()) {
        const picojson::object &camsObj = camsField->second.get<picojson::object>();
        if (auto camField = camsObj.find("Camera"); camField != camsObj.end()) {
            picojson::array cameras = asArray(camField->second);
            int nCameras = cameras.size();
            for (int i = 0; i < nCameras; i++)
            {
                if (!cameras[i].is<picojson::object>()) continue;
                const picojson::object &cam = cameras[i].get<picojson::object>();

                Vec3 pos(0.0f);
                if (auto it = cam.find("Position"); it != cam.end()) {
                    pos = Vec3(getStr(it->second));
                }

                Vec3 gaze(0.0f, 0.0f, -1.0f);
                if (auto it = cam.find("Gaze"); it != cam.end()) {
                    gaze = Vec3(getStr(it->second));
                } else if (auto it = cam.find("GazePoint"); it != cam.end()) {
                    gaze = (Vec3(getStr(it->second)) - pos).normalized();
                }

                Vec3 up(0.0f, 1.0f, 0.0f);
                if (auto it = cam.find("Up"); it != cam.end()) {
                    up = Vec3(getStr(it->second));
                }

                Camera camera(pos, gaze, up);

                if (auto it = cam.find("ImageName"); it != cam.end()) {
                    camera.setFileName(getStr(it->second));
                }

                int res_h = 100, res_v = 100;
                if (auto it = cam.find("ImageResolution"); it != cam.end()) {
                    std::stringstream ss(getStr(it->second));
                    ss >> res_h >> res_v;
                }

                float d = 1.0f;
                if (auto it = cam.find("NearDistance"); it != cam.end()) {
                    std::stringstream ss(getStr(it->second));
                    ss >> d;
                }

                float l = -1.0f, r = 1.0f, b = -1.0f, t = 1.0f;
                if (auto it = cam.find("NearPlane"); it != cam.end()) {
                    std::stringstream ss(getStr(it->second));
                    ss >> l >> r >> b >> t;
                } else if (auto it = cam.find("FovY"); it != cam.end()) {
                    float fovY = std::stof(getStr(it->second));
                    t = d * std::tan((fovY * 0.5f) * (3.14159265358979323846f / 180.0f));
                    b = -t;
                    r = t * ((float)res_h / (float)res_v);
                    l = -r;
                }

                camera.setViewPlane(l, r, b, t, d, res_h, res_v);
                scene.cameras.push_back(camera);
            }
        }
    }

    if (auto lightsField = sceneJson.find("Lights"); lightsField != sceneJson.end() && lightsField->second.is<picojson::object>()) {
        const picojson::object &lightsJson = lightsField->second.get<picojson::object>();
        if (auto field = lightsJson.find("AmbientLight"); field != lightsJson.end()) {
            Vec3 intensity = Vec3(getStr(field->second));
            scene.ambientLight = Light::createAmbient(intensity);
        }

        if (auto field = lightsJson.find("PointLight"); field != lightsJson.end()) {
            picojson::array pointLights = asArray(field->second);
            int nPointLights = pointLights.size();
            for (int i = 0; i < nPointLights; i++) {
                if (!pointLights[i].is<picojson::object>()) continue;
                const picojson::object &pointLightJson = pointLights[i].get<picojson::object>();
                Vec3 intensity(0.0f);
                if (auto it = pointLightJson.find("Intensity"); it != pointLightJson.end()) {
                    intensity = Vec3(getStr(it->second));
                }
                Vec3 position(0.0f);
                if (auto it = pointLightJson.find("Position"); it != pointLightJson.end()) {
                    position = Vec3(getStr(it->second));
                }
                scene.pointLights.push_back(Light::createPoint(intensity, position));
            }
        }
    }

    if (auto matsField = sceneJson.find("Materials"); matsField != sceneJson.end() && matsField->second.is<picojson::object>()) {
        const picojson::object &matsObj = matsField->second.get<picojson::object>();
        if (auto matField = matsObj.find("Material"); matField != matsObj.end()) {
            picojson::array materials = asArray(matField->second);
            int nMaterials = materials.size();
            for (int i = 0; i < nMaterials; i++)
            {
                if (!materials[i].is<picojson::object>()) continue;
                const picojson::object &materialJson = materials[i].get<picojson::object>();

                Vec3 ambient(0.0f);
                if (auto it = materialJson.find("AmbientReflectance"); it != materialJson.end()) {
                    ambient = Vec3(getStr(it->second));
                }

                Vec3 diffuse(0.0f);
                if (auto it = materialJson.find("DiffuseReflectance"); it != materialJson.end()) {
                    diffuse = Vec3(getStr(it->second));
                }

                Vec3 specular(0.0f);
                if (auto it = materialJson.find("SpecularReflectance"); it != materialJson.end()) {
                    specular = Vec3(getStr(it->second));
                }

                float phongExp = 1.0f;
                if (auto it = materialJson.find("PhongExponent"); it != materialJson.end()) {
                    phongExp = std::stof(getStr(it->second));
                }

                Vec3 mirror(0.0f);
                if (auto typeIt = materialJson.find("_type"); typeIt != materialJson.end() && getStr(typeIt->second) == "mirror") {
                    if (auto it = materialJson.find("MirrorReflectance"); it != materialJson.end()) {
                        mirror = Vec3(getStr(it->second));
                    }
                }

                int matId = i + 1;
                if (auto idIt = materialJson.find("_id"); idIt != materialJson.end()) {
                    matId = std::stoi(getStr(idIt->second));
                }
                if (matId > (int)scene.materials.size()) {
                    scene.materials.resize(matId, Material(Vec3(0.0f), Vec3(0.0f), Vec3(0.0f), Vec3(0.0f), 1.0f));
                }
                scene.materials[matId - 1] = Material(ambient, diffuse, specular, mirror, phongExp);
            }
        }
    }

    if (auto vdField = sceneJson.find("VertexData"); vdField != sceneJson.end()) {
        std::string vertsStr = getStr(vdField->second);
        float nums[3];
        int i = 0;
        std::stringstream ss(vertsStr);
        while (ss >> nums[i++])
        {
            if (i >= 3)
            {
                i = 0;
                scene.verts.push_back(Vec3(nums));
            }
        }
    }

    if (auto objsField = sceneJson.find("Objects"); objsField != sceneJson.end() && objsField->second.is<picojson::object>()) {
        const picojson::object &objectsJson = objsField->second.get<picojson::object>();

        if (auto field = objectsJson.find("Sphere"); field != objectsJson.end()) {
            picojson::array sphereObjects = asArray(field->second);
            int nSphereObjects = sphereObjects.size();
            for (int i = 0; i < nSphereObjects; i++) {
                if (!sphereObjects[i].is<picojson::object>()) continue;
                const picojson::object &sphereObject = sphereObjects[i].get<picojson::object>();
                int matId = std::stoi(getStr(sphereObject.at("Material")));
                int centerId = sphereObject.find("Center") != sphereObject.end()
                                   ? std::stoi(getStr(sphereObject.at("Center")))
                                   : std::stoi(getStr(sphereObject.at("Point")));
                float rad = std::stof(getStr(sphereObject.at("Radius")));
                scene.objects.push_back(new SphereObject(scene.getMaterialP(matId), scene.getVertexP(centerId), rad));
            }
        }

        if (auto field = objectsJson.find("Cylinder"); field != objectsJson.end()) {
            picojson::array cylinderObjects = asArray(field->second);
            int nCylinderObjects = cylinderObjects.size();
            for (int i = 0; i < nCylinderObjects; i++) {
                if (!cylinderObjects[i].is<picojson::object>()) continue;
                const picojson::object &cylinderObject = cylinderObjects[i].get<picojson::object>();
                int matId = std::stoi(getStr(cylinderObject.at("Material")));
                int pointId = cylinderObject.find("Center") != cylinderObject.end()
                                  ? std::stoi(getStr(cylinderObject.at("Center")))
                                  : std::stoi(getStr(cylinderObject.at("Point")));
                Vec3 axis = Vec3(getStr(cylinderObject.at("Axis")));
                float radius = std::stof(getStr(cylinderObject.at("Radius")));
                float height = std::stof(getStr(cylinderObject.at("Height")));
                scene.objects.push_back(new CylinderObject(scene.getMaterialP(matId), scene.getVertexP(pointId), axis, height, radius));
            }
        }

        if (auto field = objectsJson.find("Triangle"); field != objectsJson.end()) {
            picojson::array triangleObjects = asArray(field->second);
            int nTriangleObjects = triangleObjects.size();
            for (int i = 0; i < nTriangleObjects; i++) {
                if (!triangleObjects[i].is<picojson::object>()) continue;
                const picojson::object &triangleObject = triangleObjects[i].get<picojson::object>();
                int matId = std::stoi(getStr(triangleObject.at("Material")));
                std::string indicesStr = triangleObject.find("Indices") != triangleObject.end()
                                             ? getStr(triangleObject.at("Indices"))
                                             : getStr(triangleObject.at("Faces"));
                std::stringstream ss(indicesStr);
                int idx0, idx1, idx2;
                if (ss >> idx0 >> idx1 >> idx2) {
                    scene.objects.push_back(new TriangleObject(scene.getMaterialP(matId), scene.getVertexP(idx0), scene.getVertexP(idx1), scene.getVertexP(idx2)));
                }
            }
        }

        if (auto field = objectsJson.find("Mesh"); field != objectsJson.end()) {
            picojson::array meshObjects = asArray(field->second);
            int nmeshObjects = meshObjects.size();
            for (int i = 0; i < nmeshObjects; i++) {
                if (!meshObjects[i].is<picojson::object>()) continue;
                const picojson::object &meshObject = meshObjects[i].get<picojson::object>();
                int matId = std::stoi(getStr(meshObject.at("Material")));

                std::string indicesStr = meshObject.find("Faces") != meshObject.end()
                                             ? getStr(meshObject.at("Faces"))
                                             : getStr(meshObject.at("Indices"));
                int indices[3];
                std::stringstream ss(indicesStr);
                int j = 0;
                while (ss >> indices[j++]) {
                    if (j >= 3) {
                        scene.objects.push_back(new TriangleObject(scene.getMaterialP(matId), scene.getVertexP(indices[0]), scene.getVertexP(indices[1]), scene.getVertexP(indices[2])));
                        j = 0;
                    }
                }
            }
        }

        if (auto field = objectsJson.find("Plane"); field != objectsJson.end()) {
            picojson::array planeObjects = asArray(field->second);
            int nPlaneObjects = planeObjects.size();
            for (int i = 0; i < nPlaneObjects; i++) {
                if (!planeObjects[i].is<picojson::object>()) continue;
                const picojson::object &planeObject = planeObjects[i].get<picojson::object>();
                int matId = std::stoi(getStr(planeObject.at("Material")));
                int pointId = planeObject.find("Center") != planeObject.end()
                                  ? std::stoi(getStr(planeObject.at("Center")))
                                  : std::stoi(getStr(planeObject.at("Point")));
                Vec3 normal = Vec3(getStr(planeObject.at("Normal")));
                scene.objects.push_back(new PlaneObject(scene.getMaterialP(matId), scene.getVertexP(pointId), normal));
            }
        }
    }

    return scene;
}

Scene::~Scene()
{
    int nObjects = objects.size();
    for (int i = 0; i < nObjects; i++) {
        delete objects[i];
    }
}