#include <cmath>
#include "parser.h"
#include "intersect.h"
#include "ppm.h"
#include <vector>

// typedef unsigned char RGB[3];

unsigned char clamp(float value)
{
    if (value > 255)
        return 255;
    else if (value < 0)
        return 0;
    else
        return (unsigned char)roundf(value);
}

int main(int argc, char* argv[])
{
    // Sample usage for reading an XML scene file
    parser::Scene scene;

    scene.loadFromXml(argv[1]);

    int width, height;
    for(int c=0; c < scene.cameras.size(); c++){
        width = scene.cameras[c].image_width;
        height = scene.cameras[c].image_height;
        auto* image = new unsigned char [width * height * 3];
        height = scene.cameras[c].image_height;
        width = scene.cameras[c].image_width;

        parser::Vec4f near_plane = scene.cameras[c].near_plane;
        float left = near_plane.x;
        float right = near_plane.y;
        float bottom = near_plane.z;
        float top = near_plane.w;
        float pixel_width = (right - left) / (float)width;
        float pixel_height = (top - bottom) / (float)height;

        parser::Vec3f camera_pos = scene.cameras[c].position;
        parser::Vec3f w = scene.cameras[c].gaze * -1;
        parser::Vec3f v = scene.cameras[c].up;
        parser::Vec3f u = v.cross(w);

        #pragma omp parallel for
        for (int i = 0; i < height; ++i)
        {
            // calculating the beginning index of a "row" of pixels
            int h_displacement = i * 3 * width;
            for (int j = 0; j < width; ++j)
            {
                // calculating the beginning index of a single pixel
                int w_displacement = j * 3;
                int total_displacement = h_displacement + w_displacement;
                parser::Vec3f pixel_pos{left + (float)(j + 0.5) * pixel_width,
                                        top - (float)(i + 0.5) * pixel_height,
                                        -1 * scene.cameras[c].near_distance};
                
                // find ray direction
                parser::Vec3f d = u * pixel_pos.x + v * pixel_pos.y + w * pixel_pos.z;
                Ray ray{camera_pos, d, 0};
                // intersect with the scene and apply shading
                Vec3f color = ray.computeColor(scene);
                // write the color to the image
                image[total_displacement] = clamp(color.x);
                image[total_displacement + 1] = clamp(color.y);
                image[total_displacement + 2] = clamp(color.z);
            }
        }
        write_ppm(scene.cameras[c].image_name.c_str(), image, width, height);
        delete[] image;
    }
}
