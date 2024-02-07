#include "intersect.h"
#include "parser.h"
#include "shading.h"

using namespace parser;

float Ray::CheckTriangles(Scene &scene, int &min_object, bool &flag) const
{
    float min_t = INFINITY;
    for (int obj = 0; obj < scene.triangles.size(); obj++)
    {
        float t = this->TriangleIntersection(scene.triangles[obj], scene.vertex_data);
        if (t < 0) continue;
        else if (t < min_t)
        {
            min_t = t;
            min_object = obj;
            flag = true;
        }
    }
    return min_t;
}

float Ray::TriangleIntersection(parser::Triangle triangle, std::vector<Vec3f> &vertex_data) const
{
    const Vec3f &d = direction;
    const Vec3f &o = origin;
    if (this->direction.dot(triangle.normal) >= 0) return -1;
    Vec3f a = vertex_data[triangle.indices.v0_id - 1];
    Vec3f b = vertex_data[triangle.indices.v1_id - 1];
    Vec3f c = vertex_data[triangle.indices.v2_id - 1];
    float detM = (a.x - c.x)*((b.y - c.y)*(-d.z) - ((-d.y)*(b.z - c.z))) - (b.x-c.x)*((a.y-c.y)*(-d.z) - ((-d.y)*(a.z-c.z))) + (-d.x)*((a.y-c.y)*(b.z-c.z) - (b.y-c.y)*(a.z-c.z));
    if (detM == 0) return -1;
    float alpha = (o.x-c.x)*((b.y-c.y)*(-d.z) - ((-d.y)*(b.z-c.z))) - (b.x-c.x)*((o.y-c.y)*(-d.z) - ((-d.y)*(o.z-c.z))) + (-d.x)*((o.y-c.y)*(b.z-c.z) - (b.y-c.y)*(o.z-c.z));
    alpha = alpha/detM;
    if (alpha < 0 || alpha > 1) return -1;
    float beta = (a.x-c.x)*((o.y-c.y)*(-d.z) - ((-d.y)*(o.z-c.z))) - (o.x-c.x)*((a.y-c.y)*(-d.z) - ((-d.y)*(a.z-c.z))) + (-d.x)*((a.y-c.y)*(o.z-c.z) - (o.y-c.y)*(a.z-c.z));
    beta = beta/detM;
    if (beta < 0 || beta > 1 - alpha) return -1;
    float t = (a.x-c.x)*((b.y-c.y)*(o.z-c.z) - ((o.y-c.y)*(b.z-c.z))) - (b.x-c.x)*((a.y-c.y)*(o.z-c.z) - ((o.y-c.y)*(a.z-c.z))) + (o.x-c.x)*((a.y-c.y)*(b.z-c.z) - (b.y-c.y)*(a.z-c.z));
    t = t/detM;
    return t;
}

float Ray::ShadowTriangleIntersection(parser::Triangle triangle, std::vector<Vec3f> &vertex_data) const
{
    const Vec3f &d = direction;
    const Vec3f &o = origin;
    Vec3f a = vertex_data[triangle.indices.v0_id - 1];
    Vec3f b = vertex_data[triangle.indices.v1_id - 1];
    Vec3f c = vertex_data[triangle.indices.v2_id - 1];
    float detM = (a.x - c.x)*((b.y - c.y)*(-d.z) - ((-d.y)*(b.z - c.z))) - (b.x-c.x)*((a.y-c.y)*(-d.z) - ((-d.y)*(a.z-c.z))) + (-d.x)*((a.y-c.y)*(b.z-c.z) - (b.y-c.y)*(a.z-c.z));
    if (detM == 0) return -1;
    float alpha = (o.x-c.x)*((b.y-c.y)*(-d.z) - ((-d.y)*(b.z-c.z))) - (b.x-c.x)*((o.y-c.y)*(-d.z) - ((-d.y)*(o.z-c.z))) + (-d.x)*((o.y-c.y)*(b.z-c.z) - (b.y-c.y)*(o.z-c.z));
    alpha = alpha/detM;
    if (alpha < 0 || alpha > 1) return -1;
    float beta = (a.x-c.x)*((o.y-c.y)*(-d.z) - ((-d.y)*(o.z-c.z))) - (o.x-c.x)*((a.y-c.y)*(-d.z) - ((-d.y)*(a.z-c.z))) + (-d.x)*((a.y-c.y)*(o.z-c.z) - (o.y-c.y)*(a.z-c.z));
    beta = beta/detM;
    if (beta < 0 || beta > 1 - alpha) return -1;
    float t = (a.x-c.x)*((b.y-c.y)*(o.z-c.z) - ((o.y-c.y)*(b.z-c.z))) - (b.x-c.x)*((a.y-c.y)*(o.z-c.z) - ((o.y-c.y)*(a.z-c.z))) + (o.x-c.x)*((a.y-c.y)*(b.z-c.z) - (b.y-c.y)*(a.z-c.z));
    t = t/detM;
    return t;
}

float Ray::CheckMeshes(Scene &scene, int &min_object, int &min_face, bool &flag) const
{
    float min_t = INFINITY;
    for (int obj = 0; obj < scene.meshes.size(); obj++)
    {
        int min_face_inner;
        float t = this->MeshIntersection(scene.meshes[obj], scene.vertex_data, min_face_inner);
        if (t < 0) continue;
        else if (t < min_t)
        {
            min_t = t;
            min_object = obj;
            min_face = min_face_inner;
            flag = true;
        }
    }
    return min_t;
}

float Ray::MeshIntersection(parser::Mesh &mesh, std::vector<parser::Vec3f> &vertex_data, int &min_face) const {
    float min_t = INFINITY;
    bool flag = false;
    for (int face = 0; face < mesh.faces.size(); face++) {
        Triangle triangle{};
        triangle.indices = mesh.faces[face];
        triangle.normal = ((vertex_data[triangle.indices.v1_id - 1] - vertex_data[triangle.indices.v0_id - 1]).cross(
                vertex_data[triangle.indices.v2_id - 1] - vertex_data[triangle.indices.v0_id - 1])).normalize();
        float t;
        t = TriangleIntersection(triangle, vertex_data);
        if (t < 0) {
            continue;
        } else if (t < min_t) {
            min_t = t;
            flag = true;
            min_face = face;
        }
    }
    if (flag) return min_t;
    else return -1;
}

float Ray::CheckSpheres(parser::Scene &scene, int &min_object, bool &flag) const
{
    float min_t = INFINITY;
    for (int obj = 0; obj < scene.spheres.size(); obj++)
    {
        float t = this->SphereIntersection(scene.spheres[obj], scene.vertex_data);
        if (t < 0) continue;
        else if (t < min_t)
        {
            min_t = t;
            min_object = obj;
            flag = true;
        }
    }
    return min_t;
}

float Ray::SphereIntersection(parser::Sphere sphere, std::vector<Vec3f> &vertex_data) const
{
    Vec3f center = vertex_data[sphere.center_vertex_id-1];
    float radius = sphere.radius;
    float A = direction.dot(direction);
    float B = (direction*2).dot(origin - center);
    float C = (center - origin).dot(center - origin) - radius*radius;
    if((B*B - 4*A*C) < 0) return -1;
    float t1 = (-B + std::sqrt(B*B - 4*A*C))/(2*A);
    float t2 = (-B - std::sqrt(B*B - 4*A*C))/(2*A);
    if (t1<=t2) return t1;
    else return t2;
}

Vec3f Ray::FindIntersectionPoint(float t) const{
    return origin + direction * t;
}

bool Ray::shadowOrNot(Scene &scene) const{
    for(int obj=0; obj<scene.spheres.size(); obj++){
        float t = this->SphereIntersection(scene.spheres[obj], scene.vertex_data);
        if(t>0 && t<1) return true;
    }
    for(int obj=0; obj<scene.triangles.size(); obj++){
        float t = this->ShadowTriangleIntersection(scene.triangles[obj], scene.vertex_data);
        if(t>0 && t<1) return true;
    }
    for(int mesh=0; mesh<scene.meshes.size(); mesh++){
        Mesh cur_mesh = scene.meshes[mesh];
        for(int face=0; face < cur_mesh.faces.size(); face++){
            Face cur_face = cur_mesh.faces[face];
            Triangle triangle{};
            triangle.material_id = cur_mesh.material_id;
            triangle.indices = cur_face;
            Vec3f v0 = scene.vertex_data[triangle.indices.v0_id - 1];
            Vec3f v1 = scene.vertex_data[triangle.indices.v1_id - 1];
            Vec3f v2 = scene.vertex_data[triangle.indices.v2_id - 1];
            Vec3f vector1 = v1 - v0;
            Vec3f vector2 = v2 - v0;
            triangle.normal = (vector1.cross(vector2)).normalize();
            float t = this->ShadowTriangleIntersection(triangle, scene.vertex_data);
            if(t>0 && t<1) return true;
        }
    }
    return false;
}

Vec3f Ray::computeColor(parser::Scene &scene)
{
    if (bounce > scene.max_recursion_depth) return Vec3f{0,0,0};
    int min_s_obj;
    int min_t_obj;
    int min_m_obj;
    int min_m_face;
    bool flag = false;
    // find minimum t for spheres by iterating over all spheres:
    float minimum_s_t = CheckSpheres(scene, min_s_obj, flag);
    // find minimum t for triangles by iterating over all triangles:
    float minimum_t_t = CheckTriangles(scene, min_t_obj, flag);
    // find minimum t for meshes by iterating over all meshes (and all of their faces):
    float minimum_m_t = CheckMeshes(scene, min_m_obj, min_m_face, flag);
    if (not flag)
    {
        // if ray is primary ray, return background color
        if (bounce == 0)
        {
            Vec3i background_color = scene.background_color;
            return Vec3f{(float) background_color.x, (float) background_color.y, (float) background_color.z};
        }
        // if ray is reflected off another surface, don't return a color
        else return Vec3f{0,0,0};
    }
    else if (minimum_s_t <= minimum_t_t && minimum_s_t <= minimum_m_t) // change that: if minimum_s_t is smaller than minimum_t_t and minimum_m_t:
    {
        return applyShadingSphere(*this, minimum_s_t, min_s_obj, scene);
    }
    else if (minimum_t_t <= minimum_s_t && minimum_t_t <= minimum_m_t)
    {
        return applyShadingTriangle(*this, minimum_t_t, scene.triangles[min_t_obj], scene);
    }
    else
    {
        Mesh min_mesh = scene.meshes[min_m_obj];
        Face min_face = min_mesh.faces[min_m_face];
        Triangle mesh_t{min_mesh.material_id, min_face};
        Vec3f v0 = scene.vertex_data[mesh_t.indices.v0_id - 1];
        Vec3f v1 = scene.vertex_data[mesh_t.indices.v1_id - 1];
        Vec3f v2 = scene.vertex_data[mesh_t.indices.v2_id - 1];
        Vec3f vector1 = v1 - v0;
        Vec3f vector2 = v2 - v0;
        mesh_t.normal = (vector1.cross(vector2)).normalize();
        return applyShadingTriangle(*this, minimum_m_t, mesh_t, scene);
    }
}
