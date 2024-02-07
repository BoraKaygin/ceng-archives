#ifndef CODE_TEMPLATE_INTERSECT_H
#define CODE_TEMPLATE_INTERSECT_H

#include "parser.h"

using namespace parser;

struct Ray
{
    parser::Vec3f origin;
    parser::Vec3f direction;
    int bounce;
    float CheckTriangles(Scene &scene, int &min_object, bool &flag) const;
    float TriangleIntersection(parser::Triangle triangle, std::vector<parser::Vec3f> &vertex_data) const;
    float ShadowTriangleIntersection(parser::Triangle triangle, std::vector<Vec3f> &vertex_data) const;
    float CheckMeshes(Scene &scene, int &min_object, int &min_face, bool &flag) const;
    float MeshIntersection(parser::Mesh &mesh, std::vector<parser::Vec3f> &vertex_data, int &min_face) const;
    float CheckSpheres(parser::Scene &scene, int &min_object, bool &flag) const;
    float SphereIntersection(parser::Sphere sphere, std::vector<parser::Vec3f> &vertex_data_p) const;
    parser::Vec3f FindIntersectionPoint(float t) const;
    bool shadowOrNot(Scene &scene) const;
    Vec3f computeColor(parser::Scene &scene);
};

#endif //CODE_TEMPLATE_INTERSECT_H
