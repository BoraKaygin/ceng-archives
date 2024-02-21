//
// Created by hsnka on 12/12/2023.
//

#ifndef CENG477_HW2_CLIPPING_H
#define CENG477_HW2_CLIPPING_H

#include "Scene.h"
#include "Helpers.h"

bool visible(double den, double num, double &t_E, double &t_L);
bool LiangBarskyClipping(Vec4 &v0, Vec4 &v1, Color &c0, Color &c1);
bool isBackFace(const Vec4& v1, const Vec4& v2, const Vec4& v3);

#endif //CENG477_HW2_CLIPPING_H
