//
// Created by hsnka on 12/11/2023.
//

#ifndef RASTERIZATION_MODELINGTRANSFORMATIONS_H
#define RASTERIZATION_MODELINGTRANSFORMATIONS_H
#include "Scene.h"
#include "Helpers.h"


Matrix4 getTranslationMatrix(Translation *t);
Matrix4 getScalingMatrix(Scaling *scaling);
Matrix4 getRotationMatrix(Rotation *rotation);
void multiplyWithTranslation(Matrix4 &matrix, Translation *translation);
void multiplyWithScaling(Matrix4 &matrix, Scaling *scaling);
void multiplyWithRotation(Matrix4 &matrix, Rotation *rotation);

#endif //RASTERIZATION_MODELINGTRANSFORMATIONS_H
