#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include <iostream>
#include <string>

class Camera
{
public:
    Camera(const Vec3 &pos, const Vec3 &gaze, const Vec3 &up);

    void setViewPlane(float l, float r, float b, float t, float d, int res_h, int res_v);
    void setRes(int h, int v);
    void setFileName(std::string fileName);

    Vec3 camToWorld(const Vec3 &pos) const;

    Ray getPxRay(int x, int y) const;

    friend std::ostream &operator<<(std::ostream &ostr, const Camera &cam);

    Vec3 e;                             // camera position
    Vec3 v, u, w;                       // orthonormal basis of camera
    float l = -1, r = 1, b = -1, t = 1; // img plane rectangle side coordinates
    float vp_width, vp_height;
    Vec3 vp_pos;
    float px_width, px_height;
    float d = 1;                        // distance between camera and the image plane
    int res_v = 100, res_h = 100;
    std::string imageName = "image.ppm";
};

#endif
