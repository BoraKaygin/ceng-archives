//
// Created by hsnka on 12/13/2023.
//

#ifndef CENG477_HW2_RASTERIZATION_H
#define CENG477_HW2_RASTERIZATION_H

#include <vector>
#include "Scene.h"
#include "Helpers.h"

void draw(int x, int y, double z, Color c, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);

void LineRasterization(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);
void case1(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);
void case2(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);
void case7(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);
void case8(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);

double f(const Vec3i& p, const Vec3i& p0, const Vec3i& p1);
void TriangleRasterization(const Vec3i& p0, const Vec3i& p1, const Vec3i& p2, const Color& c0, const Color& c1, const Color& c2, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth);

#endif //CENG477_HW2_RASTERIZATION_H
