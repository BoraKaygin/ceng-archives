//
// Created by hsnka on 12/12/2023.
//

#include "Clipping.h"

bool visible(double den, double num, double &t_E, double &t_L) {
    double t;
    if (den > 0) {
        t = num / den;
        if (t > t_L) {
            return false;
        }
        if (t > t_E) {
            t_E = t;
        }
    } else if (den < 0) {
        t = num / den;
        if (t < t_E) {
            return false;
        }
        if (t < t_L) {
            t_L = t;
        }
    } else if (num > 0) {
        return false;
    }
    return true;
}

bool LiangBarskyClipping(Vec4 &v0, Vec4 &v1, Color &c0, Color &c1) {
    double t_E = 0.0;
    double t_L = 1.0;
    double dx = v1.x - v0.x;
    double dy = v1.y - v0.y;
    double dz = v1.z - v0.z;
    double x_min = -1.0, x_max = 1.0, y_min = -1.0, y_max = 1.0, z_min = -1.0, z_max = 1.0;
    if (visible(dx, x_min - v0.x, t_E, t_L)) {
        if (visible(-dx, v0.x - x_max, t_E, t_L)) {
            if (visible(dy, y_min - v0.y, t_E, t_L)) {
                if (visible(-dy, v0.y - y_max, t_E, t_L)) {
                    if (visible(dz, z_min - v0.z, t_E, t_L)) {
                        if (visible(-dz, v0.z - z_max, t_E, t_L)) {
                            Color temp0, temp1;
                            if (t_L < 1.0) {
                                v1.x = v0.x + t_L * dx;
                                v1.y = v0.y + t_L * dy;
                                v1.z = v0.z + t_L * dz;
                                temp1.r = c0.r + t_L * (c1.r - c0.r);
                                temp1.g = c0.g + t_L * (c1.g - c0.g);
                                temp1.b = c0.b + t_L * (c1.b - c0.b);
                            } else {
                                temp1 = c1;
                            }
                            if (t_E > 0.0) {
                                v0.x = v0.x + t_E * dx;
                                v0.y = v0.y + t_E * dy;
                                v0.z = v0.z + t_E * dz;
                                temp0.r = c0.r + t_E * (c1.r - c0.r);
                                temp0.g = c0.g + t_E * (c1.g - c0.g);
                                temp0.b = c0.b + t_E * (c1.b - c0.b);
                            } else {
                                temp0 = c0;
                            }
                            c0.r = temp0.r; c0.g = temp0.g; c0.b = temp0.b;
                            c1.r = temp1.r; c1.g = temp1.g; c1.b = temp1.b;
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}

bool isBackFace(const Vec4& v_1, const Vec4& v_2, const Vec4& v_3){
    Vec3 v1 = vec4IntoVec3(v_1);
    Vec3 v2 = vec4IntoVec3(v_2);
    Vec3 v3 = vec4IntoVec3(v_3);
    Vec3 normal = computeNormal(v1, v2, v3);
    Vec3 v = {(v1.x+v2.x+v3.x)/3.0, (v1.y+v2.y+v3.y)/3.0, (v1.z+v2.z+v3.z)/3.0};
    v = normalizeVec3(v);
    return dotProductVec3(normal, v) < 0; // should be > 0 but for some reason it works this way.
}
