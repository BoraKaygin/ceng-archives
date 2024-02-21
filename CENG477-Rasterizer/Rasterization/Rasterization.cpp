//
// Created by hsnka on 12/13/2023.
//
#include "Rasterization.h"
#include "Helpers.h"
#include <cmath>

void draw(int x, int y, double z, Color c, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth){
    if (z < depth[x][y]) {
        image[x][y] = c;
        depth[x][y] = z;
    }
}

void case1(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth){
    int y = p0.y;
    int m = 2*(p0.y - p1.y);
    int n = 2*(p1.x - p0.x);
    int d = 2*(p0.y - p1.y) + (p1.x - p0.x);
    Color c = c0;
    Color dc = (c1-c0)/(p1.x-p0.x);
    double z = p0.z;
    double dz = (p1.z-p0.z)/(p1.x-p0.x);
    for(int x=p0.x; x<=p1.x; x++){
        draw(x,y,z,c,image,depth);
        if(d<0){
            y++;
            d+=m+n;
        }
        else{
            d+=m;
        }
        c = c + dc;
        z = z + dz;
    }
}

void case2(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth){
    int x = p0.x;
    int m = 2*(p0.y - p1.y);
    int n = 2*(p1.x - p0.x);
    int d = (p0.y - p1.y) + 2*(p1.x - p0.x);
    Color c = c0;
    Color dc = (c1-c0)/(p1.y-p0.y);
    double z = p0.z;
    double dz = (p1.z-p0.z)/(p1.y-p0.y);
    for(int y=p0.y; y<=p1.y; y++){
        draw(x,y,z,c,image,depth);
        if(d>0){
            x++;
            d+=m+n;
        }
        else{
            d+=n;
        }
        c = c + dc;
        z = z + dz;
    }
}

void case7(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth){
    int x = p0.x;
    int m = 2*(p0.y - p1.y);
    int n = 2*(p1.x - p0.x);
    int d = (p0.y - p1.y) + 2*(p1.x - p0.x);
    Color c = c0;
    Color dc = (c1-c0)/(p0.y-p1.y);
    double z = p0.z;
    double dz = (p1.z-p0.z)/(p0.y-p1.y);
    for(int y=p0.y; y>=p1.y; y--){
        draw(x,y,z,c,image,depth);
        if(d<0){
            x++;
            d+=m-n;
        }
        else{
            d+=-n;
        }
        c = c + dc;
        z = z + dz;
    }
}

void case8(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth){
    int y = p0.y;
    int m = 2*(p0.y - p1.y);
    int n = 2*(p1.x - p0.x);
    int d = 2*(p0.y - p1.y) - (p1.x - p0.x);
    Color c = c0;
    Color dc = (c1-c0)/(p1.x-p0.x);
    double z = p0.z;
    double dz = (p1.z-p0.z)/(p1.x-p0.x);
    for(int x=p0.x; x<=p1.x; x++){
        draw(x,y,z,c,image,depth);
        if(d>0){
            y--;
            d+=m-n;
        }
        else{
            d+=m;
        }
        c = c + dc;
        z = z + dz;
    }
}

void LineRasterization(const Vec3i& p0, const Vec3i& p1, const Color& c0, const Color& c1, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth) {
    if (p0.x == p1.x && p0.y == p1.y) {
        if (p0.z < p1.z) draw(p0.x, p0.y, p0.z, c0, image, depth);
        else draw(p1.x, p1.y, p1.z, c1, image, depth);
        return;
    }
    bool goingRight = p1.x > p0.x;
    bool goingUp = p1.y > p0.y;
    bool steep = abs(p1.y - p0.y) > abs(p1.x - p0.x);
    if(goingRight && goingUp && !steep) case1(p0,p1,c0,c1, image, depth);
    else if(goingRight && goingUp && steep) case2(p0,p1,c0,c1, image, depth);
    else if(!goingRight && goingUp && steep) case7(p1,p0,c1,c0, image, depth);
    else if(!goingRight && goingUp && !steep) case8(p1,p0,c1,c0, image, depth);
    else if(!goingRight && !goingUp && !steep) case1(p1,p0,c1,c0, image, depth);
    else if(!goingRight && !goingUp && steep) case2(p1,p0,c1,c0, image, depth);
    else if(goingRight && !goingUp && steep) case7(p0,p1,c0,c1, image, depth);
    else case8(p0,p1,c0,c1,image,depth);
}

double f(const Vec3i& p, const Vec3i& p0, const Vec3i& p1){
    return (p.x-p0.x)*(p0.y-p1.y) + (p.y-p0.y)*(p1.x-p0.x);
}

void TriangleRasterization(const Vec3i &p0, const Vec3i &p1, const Vec3i &p2, const Color &c0, const Color &c1, const Color &c2, std::vector<std::vector<Color>> &image, std::vector<std::vector<double>> &depth) {
    int y_min = std::min(std::min(p0.y, p1.y), p2.y);
    int y_max = std::max(std::max(p0.y, p1.y), p2.y);
    int x_min = std::min(std::min(p0.x, p1.x), p2.x);
    int x_max = std::max(std::max(p0.x, p1.x), p2.x);
    for (int y = y_min; y <= y_max; y++){
        for (int x = x_min; x <= x_max; x++){
            Vec3i p = {x, y, 0};
            double alpha = f(p, p1, p2) / f(p0, p1, p2);
            double beta = f(p, p2, p0) / f(p1, p2, p0);
            double gamma = f(p, p0, p1) / f(p2, p0, p1);
            if (alpha >= 0 && beta >= 0 && gamma >= 0){
                double z = alpha * p0.z + beta * p1.z + gamma * p2.z;
                Color c;
                c.r = alpha * c0.r + beta * c1.r + gamma * c2.r;
                c.g = alpha * c0.g + beta * c1.g + gamma * c2.g;
                c.b = alpha * c0.b + beta * c1.b + gamma * c2.b;
                draw(x, y, z, c, image, depth);
            }
        }
    }
}


#include "Rasterization.h"
