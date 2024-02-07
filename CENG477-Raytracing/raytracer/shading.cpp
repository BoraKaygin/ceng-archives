#include "shading.h"

using namespace parser;

Vec3f ambientShading(Material &material, Scene &scene) { // material -1 ??
    // find k_a:
    Vec3f k_a = material.ambient;
    // find L_a:
    Vec3f L_a = scene.ambient_light;
    return k_a * L_a;
}

Vec3f applyShadingSphere(Ray &ray, float &t, int &sphere_id, Scene &scene)
{
    // find intersection point:
    Sphere sphere = scene.spheres[sphere_id];
    Vec3f intersection_point = ray.FindIntersectionPoint(t);
    //find normal at the intersection point:
    Vec3f sphere_center = scene.vertex_data[sphere.center_vertex_id-1];
    Vec3f sphere_normal = (intersection_point - sphere_center).normalize();

    // add ambient shading
    Vec3f totalL_0 = ambientShading(scene.materials[sphere.material_id - 1], scene);

    // add ideal specular shading (mirror reflection) ((if material is mirror))
    if(scene.materials[sphere.material_id - 1].is_mirror){
        // find k_m
        Vec3f k_m = scene.materials[sphere.material_id-1].mirror;
        // find L_i:
        // find w_o, w_r and call compute_color
        Vec3f w_o = (ray.origin - intersection_point).normalize();
        Vec3f w_r = (sphere_normal * 2 * sphere_normal.dot(w_o) - w_o).normalize();
        Ray reflected_ray{intersection_point + sphere_normal * scene.shadow_ray_epsilon, w_r, ray.bounce + 1};
        Vec3f color = reflected_ray.computeColor(scene) * k_m;
        totalL_0.x += color.x;
        totalL_0.y += color.y;
        totalL_0.z += color.z;
    }

    // then for every point light add diffuse and specular shading:
    for (int p_l=0; p_l< scene.point_lights.size(); p_l++){
        Vec3f pl_position = scene.point_lights[p_l].position;
        Vec3f pl_intensity = scene.point_lights[p_l].intensity;
        // check if point is in the shadow wrt this p_l
        Vec3f intersect_point_withE = intersection_point + (sphere_normal * scene.shadow_ray_epsilon);
        Ray shadow_ray{intersect_point_withE, pl_position-intersect_point_withE};
        bool inShadow = shadow_ray.shadowOrNot(scene);
        if(inShadow) continue;
        // find l vector
        Vec3f l_vector =(pl_position - intersection_point).normalize();
        // find cos_theta
        float cos_theta = l_vector.dot(sphere_normal);
        if(cos_theta <= 0) continue;
        // find L_i:
        float dist = (pl_position - intersection_point).length();
        Vec3f L_i = pl_intensity / (dist*dist);

        // calculate diffuse shading:
        // calculate k_d:
        Vec3f k_d = scene.materials[sphere.material_id-1].diffuse;
        // find L_0d:
        Vec3f L_0d = L_i * k_d * cos_theta;
        totalL_0.x += L_0d.x;
        totalL_0.y += L_0d.y;
        totalL_0.z += L_0d.z;

        // calculate specular shading:
        // calculate v (view direction)
        Vec3f v_vector = (ray.origin - intersection_point).normalize();
        // calculate h (middle vector between v and l)
        Vec3f h_vector = (l_vector + v_vector).normalize();
        // find cos_alpha
        float cos_alpha = h_vector.dot(sphere_normal);
        // find k_s
        Vec3f k_s = scene.materials[sphere.material_id-1].specular;
        // find phong exponent
        float phong = scene.materials[sphere.material_id-1].phong_exponent;
        // find L_0s
        Vec3f L_0s = L_i * k_s * std::pow(cos_alpha, phong);
        totalL_0.x += L_0s.x;
        totalL_0.y += L_0s.y;
        totalL_0.z += L_0s.z;
    }
    return totalL_0;
}

Vec3f applyShadingTriangle(Ray &ray, float &t, Triangle &triangle, Scene &scene)
{
    // find intersection point:
    Vec3f intersection_point = ray.FindIntersectionPoint(t);
    //find normal at the intersection point:
    Vec3f triangle_normal = triangle.normal;

    // add ambient shading
    Vec3f totalL_0 = ambientShading(scene.materials[triangle.material_id - 1], scene);

    // add ideal specular shading (mirror reflection) ((if material is mirror))
    if(scene.materials[triangle.material_id - 1].is_mirror){
        // find k_m
        Vec3f k_m = scene.materials[triangle.material_id-1].mirror;
        // find L_i:
        // find w_o, w_r and call compute_color
        Vec3f w_o = (ray.origin - intersection_point).normalize();
        Vec3f w_r = (triangle_normal * 2 * triangle_normal.dot(w_o) - w_o).normalize();
        Ray reflected_ray{intersection_point + triangle_normal * scene.shadow_ray_epsilon, w_r, ray.bounce + 1};
        Vec3f color = reflected_ray.computeColor(scene) * k_m;
        totalL_0.x += color.x;
        totalL_0.y += color.y;
        totalL_0.z += color.z;
    }

    // then for every point light add diffuse and specular shading:
    for (int p_l=0; p_l< scene.point_lights.size(); p_l++){
        Vec3f pl_position = scene.point_lights[p_l].position;
        Vec3f pl_intensity = scene.point_lights[p_l].intensity;
        // check if point is in the shadow wrt this p_l
        Vec3f intersect_point_withE = intersection_point + (triangle_normal * scene.shadow_ray_epsilon);
        Ray shadow_ray{intersect_point_withE, pl_position-intersect_point_withE};
        bool inShadow = shadow_ray.shadowOrNot(scene);
        if(inShadow) continue;
        // find l vector
        Vec3f l_vector =(pl_position - intersection_point).normalize();
        // find cos_theta
        float cos_theta = l_vector.dot(triangle_normal);
        if(cos_theta <= 0) continue;
        // find L_i:
        float dist = (pl_position - intersection_point).length();
        Vec3f L_i = pl_intensity / (dist*dist);

        // calculate diffuse shading:
        // calculate k_d:
        Vec3f k_d = scene.materials[triangle.material_id-1].diffuse;
        // find L_0d:
        Vec3f L_0d = L_i * k_d * cos_theta;
        totalL_0.x += L_0d.x;
        totalL_0.y += L_0d.y;
        totalL_0.z += L_0d.z;

        // calculate specular shading:
        // calculate v (view direction)
        Vec3f v_vector = (ray.origin - intersection_point).normalize();
        // calculate h (middle vector between v and l)
        Vec3f h_vector = (l_vector + v_vector).normalize();
        // find cos_alpha
        float cos_alpha = h_vector.dot(triangle_normal);
        // find k_s
        Vec3f k_s = scene.materials[triangle.material_id-1].specular;
        // find phong exponent
        float phong = scene.materials[triangle.material_id-1].phong_exponent;
        // find L_0s
        Vec3f L_0s = L_i * k_s * std::pow(cos_alpha, phong);
        totalL_0.x += L_0s.x;
        totalL_0.y += L_0s.y;
        totalL_0.z += L_0s.z;
    }
    return totalL_0;
}
