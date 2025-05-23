#include <iomanip>
#include <cmath>
#include "Vec3.h"

Vec3::Vec3()
{
    this->x = 0.0;
    this->y = 0.0;
    this->z = 0.0;
    this->colorId = NO_COLOR;
}

Vec3::Vec3(double x, double y, double z)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = NO_COLOR;
}

Vec3::Vec3(double x, double y, double z, int colorId)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->colorId = colorId;
}

Vec3::Vec3(const Vec3 &other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
    this->colorId = other.colorId;
}

double Vec3::getNthComponent(int n)
{
    switch (n)
    {
    case 0:
        return this->x;

    case 1:
        return this->y;

    case 2:
    default:
        return this->z;
    }
}

std::ostream &operator<<(std::ostream &os, const Vec3 &v)
{
    os << std::fixed << std::setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}

Vec3i::Vec3i() {
    this->x = 0;
    this->y = 0;
    this->z = 0;
}

Vec3i::Vec3i(int x, int y, double z) {
    this->x = x;
    this->y = y;
    this->z = z;
}

Vec3i::Vec3i(const Vec3i &other) {
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

/*
Vec3i::Vec3i(const Vec4 &other) {
    if (other.x == -0.5) this->x = 0;
    else this->x = (int) round(other.x);
    if (other.y == -0.5) this->y = 0;
    else this->y = (int) round(other.y);
    this->z = other.z;
}
 */

Vec3i::Vec3i(const Vec4 &other){
    this->x = (int) round(other.x);
    this->y = (int) round(other.y);
    this->z = other.z;
}

std::ostream &operator<<(std::ostream &os, const Vec3i &v) {
    os << std::fixed << std::setprecision(6) << "[" << v.x << ", " << v.y << ", " << v.z << "]";
    return os;
}
