#include <iomanip>
#include "Color.h"

Color::Color() {
    this->r = 0;
    this->g = 0;
    this->b = 0;
}

Color::Color(double r, double g, double b)
{
    this->r = r;
    this->g = g;
    this->b = b;
}

Color::Color(const Color &other)
{
    this->r = other.r;
    this->g = other.g;
    this->b = other.b;
}

std::ostream &operator<<(std::ostream &os, const Color &c)
{
    os << std::fixed << std::setprecision(0) << "rgb(" << c.r << ", " << c.g << ", " << c.b << ")";
    return os;
}

Color Color::operator+(const Color &c) const {
    return {this->r + c.r, this->g + c.g, this->b + c.b};
}

Color Color::operator-(const Color &c) const {
    return {this->r - c.r, this->g - c.g, this->b - c.b};
}

Color Color::operator/(const int &i) const {
    return {this->r / i, this->g / i, this->b / i};
}
