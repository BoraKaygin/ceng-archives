//
// Created by hsnka on 12/12/2023.
//

#ifndef CENG477_HW2_VIEWINGTRANSFORMATIONS_H
#define CENG477_HW2_VIEWINGTRANSFORMATIONS_H
#include "Scene.h"
#include "Helpers.h"

Matrix4 getCameraTransformationMatrix(Camera *camera);
Matrix4 getOrthographicProjectionMatrix(Camera *camera);
Matrix4 getPerspectiveProjectionMatrix(Camera *camera);

#endif //CENG477_HW2_VIEWINGTRANSFORMATIONS_H
