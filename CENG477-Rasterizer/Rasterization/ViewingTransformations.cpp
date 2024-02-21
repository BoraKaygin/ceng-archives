//
// Created by hsnka on 12/12/2023.
//

#include "ViewingTransformations.h"

Matrix4 getCameraTransformationMatrix(Camera *camera){
    double ux = camera->u.x; double uy = camera->u.y; double uz = camera->u.z;
    double vx = camera->v.x; double vy = camera->v.y; double vz = camera->v.z;
    double wx = camera->w.x; double wy = camera->w.y; double wz = camera->w.z;
    double ex = camera->position.x; double ey = camera->position.y; double ez = camera->position.z;

    double M_cam[4][4] = {
            {ux, uy, uz, -(ux*ex + uy*ey + uz*ez)},
            {vx, vy, vz, -(vx*ex + vy*ey + vz*ez)},
            {wx, wy, wz, -(wx*ex + wy*ey + wz*ez)},
            {0, 0, 0, 1}
    };
    return Matrix4(M_cam);
}

Matrix4 getOrthographicProjectionMatrix(Camera *camera){
    double r = camera->right;
    double l = camera->left;
    double t = camera->top;
    double b = camera->bottom;
    double n = camera->near;
    double f = camera->far;

    double M_orth[4][4] = {
            {2/(r-l),0,0,-(r+l)/(r-l)},
            {0, 2/(t-b), 0, -(t+b)/(t-b)},
            {0, 0, -2/(f-n), -(f+n)/(f-n)},
            {0, 0, 0, 1}
    };
    return Matrix4(M_orth);
}

Matrix4 getPerspectiveProjectionMatrix(Camera *camera){
    double r = camera->right;
    double l = camera->left;
    double t = camera->top;
    double b = camera->bottom;
    double n = camera->near;
    double f = camera->far;

    double M_per[4][4] = {
            {2*n/(r-l), 0, (r+l)/(r-l),0},
            {0, 2*n/(t-b), (t+b)/(t-b), 0},
            {0, 0, -(f+n)/(f-n), -(2*f*n)/(f-n)},
            {0, 0, -1, 0}
    };
    return Matrix4(M_per);
}
