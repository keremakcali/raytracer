
#include <cstdint>
#include <fstream>
#include <iostream>
#include <string>
#include "scene.h"
#include "raytracer.h"

void writeImage(const Bitmap &bitmap, const std::string &filename) {
    std::ofstream outFile(filename, std::ios::binary);
    outFile << "P6\n";
    outFile << bitmap.width << " " << bitmap.height << "\n";
    outFile << "255\n";
    outFile.write(reinterpret_cast<const char *>(bitmap.pixels.data()), bitmap.pixels.size() * sizeof(Color));
}

int main(int argc, char **argv) {
    if (argc != 4 || std::string(argv[2]) != "-o") {
        std::cerr << "Usage is: raytracer <filename> -o <image_out>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    Scene scene = Scene::parseSceneJson(file);
    RayTracer rt = RayTracer(scene);
    Bitmap bitmap = rt.render();
    writeImage(bitmap, argv[3]);
}