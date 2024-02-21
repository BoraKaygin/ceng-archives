//
// Created by hsnka on 12/11/2023.
//

#include "ModelingTransformations.h"
#include <cmath>
#define PI 3.14159265358979323846

Matrix4 getTranslationMatrix(Translation *translation){
    Matrix4 translation_matrix = getIdentityMatrix();
    translation_matrix.values[0][3] = translation->tx;
    translation_matrix.values[1][3] = translation->ty;
    translation_matrix.values[2][3] = translation->tz;
    return translation_matrix;
}

Matrix4 getScalingMatrix(Scaling *scaling){
    Matrix4 scaling_matrix = Matrix4();
    scaling_matrix.values[0][0] = scaling->sx;
    scaling_matrix.values[1][1] = scaling->sy;
    scaling_matrix.values[2][2] = scaling->sz;
    scaling_matrix.values[3][3] = 1;
    return scaling_matrix;
}

Matrix4 getRotationMatrix(Rotation *rotation){
    //Matrix4 rotation_matrix = Matrix4();
    Vec3 u = Vec3(rotation->ux, rotation->uy, rotation->uz);
    double absUx = std::abs(u.x);
    double absUy = std::abs(u.y);
    double absUz = std::abs(u.z);
    Vec3 v;
    if(absUx <= absUy && absUx <= absUz){
        v.x = 0;
        v.y = -u.z;
        v.z = u.y;
    } else if (absUy <= absUx && absUy <= absUz){
        v.x = -u.z;
        v.y = 0;
        v.z = u.x;
    } else {
        v.x = -u.y;
        v.y = u.x;
        v.z = 0;
    }
    Vec3 w = crossProductVec3(u, v);
    u = normalizeVec3(u);
    v = normalizeVec3(v);
    w = normalizeVec3(w);
    double M[4][4] = {
            {u.x, u.y, u.z, 0},
            {v.x, v.y, v.z, 0},
            {w.x, w.y, w.z, 0},
            {0, 0, 0, 1}
    };
    Matrix4 alignAxis(M);
    double MInverse[4][4] = {
            {u.x, v.x, w.x, 0},
            {u.y, v.y, w.y, 0},
            {u.z, v.z, w.z, 0},
            {0, 0, 0, 1}
    };
    Matrix4 alignBack(MInverse);
    double angle = rotation->angle;
    angle = angle / 180 * PI;
    Matrix4 matrix_rx = getIdentityMatrix();
    matrix_rx.values[1][1] = std::cos(angle);
    matrix_rx.values[1][2] = (-1)*std::sin(angle);
    matrix_rx.values[2][1] = std::sin(angle);
    matrix_rx.values[2][2] = std::cos(angle);
    Matrix4 rotation_matrix = multiplyMatrixWithMatrix(matrix_rx, alignAxis);
    rotation_matrix = multiplyMatrixWithMatrix(alignBack, rotation_matrix);
    return rotation_matrix;
}

void multiplyWithTranslation(Matrix4 &matrix, Translation *translation){
    Matrix4 translation_matrix = getTranslationMatrix(translation);
    matrix = multiplyMatrixWithMatrix(matrix, translation_matrix);
}

void multiplyWithScaling(Matrix4 &matrix, Scaling *scaling){
    Matrix4 scaling_matrix = getScalingMatrix(scaling);
    matrix = multiplyMatrixWithMatrix(matrix, scaling_matrix);
}

void multiplyWithRotation(Matrix4 &matrix, Rotation *rotation){
    Matrix4 rotation_matrix = getRotationMatrix(rotation);
    matrix = multiplyMatrixWithMatrix(matrix, rotation_matrix);
}