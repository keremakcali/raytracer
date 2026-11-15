#include "camera.h"

Camera::Camera(const Vec3 &pos, const Vec3 &gaze, const Vec3 &up)
{
    e = pos;
    w = -gaze.normalized();
    u = (up - gaze * up.dot(gaze) / (gaze.length_squared())).normalized(); // orthonormal component of up wrt gaze in case they are not orthogonal
    // ^^^^ i am not handlign if up and gaze are paralel here, i might need to check that i duno
    v = u.cross(w);
}

Ray Camera::getPxRay(int x, int y) const
{
    Vec3 samplePoint = camToWorld(vp_pos + Vec3((x + 0.5) * px_width, - (y + 0.5) * px_height, 0));
    Vec3 eyeRay = (samplePoint - e).normalized();
    return Ray(samplePoint, eyeRay);
}

Vec3 Camera::camToWorld(const Vec3 &pos) const{
    return e + v * pos.x() + u * pos.y() + w * pos.z();
}

void Camera::setViewPlane(float l, float r, float b, float t, float d, int res_h, int res_v)
{
    this->l = l;
    this->r = r;
    this->b = b;
    this->t = t;
    this->d = d;
    this->res_h = res_h;
    this->res_v = res_v;
    vp_width = r - l;
    vp_height = t - b;
    px_width = vp_width / res_h;
    px_height = vp_height / res_v;
    vp_pos = Vec3(l, t, -d);
}

void Camera::setRes(int h, int v)
{
    res_h = h;
    res_v = v;
}

void Camera::setFileName(std::string fileName) {
    imageName = fileName;
}

std::ostream &operator<<(std::ostream &ostr, const Camera &cam) {
    return ostr << "Camera(" << "\n\t" << cam.e << "\n\t" << cam.v << " " << cam.u << " " << cam.w << "\n\t"
        << cam.l << " " << cam.r << " " << cam.b << " " << cam.t << "\n\t" << cam.d << "\n\t" << cam.res_h << " "
        << cam.res_v  << "\n\t" << cam.imageName << std::endl;
}
