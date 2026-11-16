
#include <fstream>
#include <iostream>
#include "scene.h"
#include "raytracer.h"

int main(int argc, char **argv) {

    if (argc != 2) {
        std::cerr << "Usage is: raytracer <filename>";
        return 1;
    }

    std::ifstream file(argv[1]);
    Scene scene = Scene::parseSceneJson(file);
    RayTracer rt = RayTracer(scene);
    rt.render();
}