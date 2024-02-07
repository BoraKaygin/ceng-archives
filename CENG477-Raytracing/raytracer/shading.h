#ifndef CODE_TEMPLATE_SHADING_H
#define CODE_TEMPLATE_SHADING_H

#include "parser.h"
#include "intersect.h"

using namespace parser;

Vec3f ambientShading(Material &material, Scene &scene);
Vec3f applyShadingSphere(Ray &ray, float &t, int &sphere_id, Scene &scene);
Vec3f applyShadingTriangle(Ray &ray, float &t, Triangle &triangle, Scene &scene);

#endif //CODE_TEMPLATE_SHADING_H
