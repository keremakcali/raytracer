#include "common.h"

#include <cmath>
#include <sstream>

Vec3::Vec3() : vals{0.0f, 0.0f, 0.0f} {}

Vec3::Vec3(float x, float y, float z) : vals{x, y, z} {}

Vec3::Vec3(float v) : vals{v, v, v} {}

Vec3::Vec3(float xyz[3]) : vals{xyz[0], xyz[1], xyz[2]} {}

Vec3::Vec3(std::string xyz) {
    std::stringstream stream(xyz);
    stream >> vals[0];
    stream >> vals[1];
    stream >> vals[2];
}

float Vec3::length() const {
    return std::sqrt(length_squared());
}

Vec3 Vec3::normalized() const {
    return (*this) / length();
}

float Vec3::dot(const Vec3 &other) const {
    return vals[0] * other[0] + vals[1] * other[1] + vals[2] * other[2];
}

Vec3 Vec3::cross(const Vec3 &other) const {
    return Vec3(
        vals[1] * other[2] - vals[2] * other[1],
        vals[2] * other[0] - vals[0] * other[2],
        vals[0] * other[1] - vals[1] * other[0]
    );
}

Vec3 Vec3::proj(const Vec3 &other) const {
    return this->dot(other) / (other.length_squared()) * other;
}

float Vec3::length_squared() const {
    return vals[0] * vals[0] + vals[1] * vals[1] + vals[2] * vals[2];
}

Vec3 &Vec3::operator+=(const Vec3 &other) {
    vals[0] += other[0];
    vals[1] += other[1];
    vals[2] += other[2];
    return *this;
}

Vec3 Vec3::operator-() const {
    return Vec3(-vals[0], -vals[1], -vals[2]);
}

Vec3 operator+(const Vec3 &vec1, const Vec3 &vec2) {
    return Vec3(vec1[0] + vec2[0], vec1[1] + vec2[1], vec1[2] + vec2[2]);
}

Vec3 operator-(const Vec3 &vec1, const Vec3 &vec2) {
    return vec1 + -vec2;
}

Vec3 operator*(const Vec3 &vec, const float c) {
    return Vec3(vec[0] * c, vec[1] * c, vec[2] * c);
}

Vec3 operator*(const float c, const Vec3 &vec) {
    return Vec3(vec[0] * c, vec[1] * c, vec[2] * c);
}

Vec3 operator*(const Vec3 &vec1, const Vec3 &vec2) {
    return Vec3(vec1[0] * vec2[0], vec1[1] * vec2[1], vec1[2] * vec2[2]);
}

Vec3 operator/(const Vec3 &vec, float c) {
    return vec * (1 / c);
}

std::ostream &operator<<(std::ostream &stream, const Vec3 &vec) {
    return stream << "(" << vec[0] << ", " << vec[1] << ", " << vec[2] << ")";
}

float &Vec3::operator[](size_t i) {
    return vals[i];
}

const float &Vec3::operator[](size_t i) const {
    return vals[i];
}

Vec3 Vec3::clip(float upperLimit) const {
    return Vec3(std::fmin(vals[0], upperLimit), std::fmin(vals[1], upperLimit), std::fmin(vals[2], upperLimit));
}

Vec3 Vec3::round() const {
    return Vec3(std::round(vals[0]), std::round(vals[1]), std::round(vals[2]));
}

Ray::Ray(const Vec3 &origin, const Vec3 &direction) : o(origin), d(direction) {
    if (std::abs(d.length() - 1) > 0.001) {
        throw MathException("Ray direction must be normalized");
    }
}

Vec3 Ray::at(float t) const {
    return o + t * d;
}

Ray Ray::proj(const Vec3 &n, const Vec3 &p) {
    Vec3 o_proj = o - o.proj(n);
    Vec3 dir_proj = d - d.proj(n);
    return Ray(o - o.proj(n), d - d.proj(n));
}
