#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <string>

class Vec3 {
public:
    Vec3();
    Vec3(float x, float y, float z);
    Vec3(float v);
    Vec3(float xyz[3]);
    Vec3(std::string xyz);

    float length() const;
    Vec3 normalized() const;
    float dot(const Vec3 &other) const;
    Vec3 cross(const Vec3 &other) const;
    Vec3 proj(const Vec3 &other) const;
    float length_squared() const;

    Vec3 &operator+=(const Vec3 &other);
    Vec3 operator-() const;

    friend Vec3 operator+(const Vec3 &vec1, const Vec3 &vec2);
    friend Vec3 operator-(const Vec3 &vec1, const Vec3 &vec2);
    friend Vec3 operator*(const Vec3 &vec, const float c);
    friend Vec3 operator*(const float c, const Vec3 &vec);
    friend Vec3 operator*(const Vec3 &vec1, const Vec3 &vec2);
    friend Vec3 operator/(const Vec3 &vec, float c);
    friend std::ostream &operator<<(std::ostream &stream, const Vec3 &vec);

    float &operator[](size_t i);
    const float &operator[](size_t i) const;

    Vec3 clip(float upperLimit) const;
    Vec3 round() const;

    const float &x() const { return vals[0]; }
    const float &y() const { return vals[1]; }
    const float &z() const { return vals[2]; }

    const float &r() const { return vals[0]; }
    const float &g() const { return vals[1]; }
    const float &b() const { return vals[2]; }

private:
    float vals[3];
};

#endif
