#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <string_view>
#include <cassert>
#include <utility>
#include <vector>

#include "tgaimage.h"
#include "model.h"

#define NDEBUG

constexpr TGAColor white   = {255, 255, 255, 255}; // attention, BGRA order
constexpr TGAColor green   = {  0, 255,   0, 255};
constexpr TGAColor red     = {  0,   0, 255, 255};
constexpr TGAColor blue    = {255, 128,  64, 255};
constexpr TGAColor yellow  = {  0, 200, 255, 255};

bool static fixOrientation(int &ax, int &ay, int &bx, int &by) {
    // line is steep if the difference between the x coords is less 
    // than the difference between the y coords
    bool steep = std::abs(ax - bx) < std::abs(ay - by);
    if (steep) {
        // if the line is steep, we transpose the image
        std::swap(ax, ay);
        std::swap(bx, by);
    }
    if (ax > bx) {
        // always left to right
        std::swap(ax, bx);
        std::swap(ay, by);
    }
    return steep;
}

void static nextPos(int& y, int& ierror, const int ax, const int ay, const int bx, const int by) {
    // finds next pos across connecting two vertices
    ierror += 2 * std::abs(by - ay);
    y += (by > ay ? 1 : -1) * (ierror > bx - ax);
    ierror -= 2 * (bx - ax) * (ierror > bx - ax);
}

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, const TGAColor color) {
    bool steep = fixOrientation(ax, ay, bx, by);
    int y = ay;
    int ierror = 0;
    for (int x = ax; x <= bx; x++) {
        if (steep) // if transposed, de-transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
        nextPos(y, ierror, ax, ay, bx, by);
    }
}

Model getData(std::string_view path, const int width = 64, const int height = 64) {
    std::ifstream in(path.data(), std::ifstream::in);
    if (!in.is_open()) {
        std::cerr << "Failed to open file " << path << '\n';
        return Model();
    }
    Model returnVal(width, height);
    std::string line, typeOfData, xPos, yPos, zPos;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream data(line);
        data >> typeOfData;
        // handle vertices
        if (typeOfData == "v") {
            data >> xPos;
            data >> yPos;
            data >> zPos;
            returnVal.addv(stof(xPos), stof(yPos), stof(zPos));
        }
        // handle faces
        else if (typeOfData == "f") {
            data >> xPos;
            data >> yPos;
            data >> zPos;
            std::stringstream xWord(xPos), yWord(yPos), zWord(zPos);
            returnVal.addf(stoi(xPos) - 1, stoi(yPos) - 1, stoi(zPos) - 1);
        }
    }
    return returnVal;
}

vec3i static floatToInt(const vec3f vec) {
    return vec3i(vec[0], vec[1], vec[2]);
}

vec3f static intToFloat(const vec3i vec) {
    return vec3f(vec[0], vec[1], vec[2]);
}


void wireframe(const vec3i a, const vec3i b, const vec3i c, TGAImage& framebuffer, const TGAColor color) {
    auto [ax, ay, az] = a;
    auto [bx, by, bz] = b;
    auto [cx, cy, cz] = c;
    line(ax, ay, bx, by, framebuffer, color);
    line(cx, cy, bx, by, framebuffer, color);
    line(cx, cy, ax, ay, framebuffer, color);

    framebuffer.set(ax, ay, white);
    framebuffer.set(bx, by, white);
    framebuffer.set(cx, cy, white);
}

double signed_triangle_area(const vec3i a, const vec3i b, const vec3i c) {
    return 0.5 * (
         (b.y - a.y) * (b.x + a.x) 
       + (c.y - b.y) * (c.x + b.x)
       + (a.y - c.y) * (a.x + c.x));
} 

void triangle(vec3i a, vec3i b, vec3i c, TGAImage& framebuffer, const TGAColor color) {
    // get the lower left corner and
    // upper right corner of the bounding box
    int bbminx = std::min({ a.x, b.x, c.x });
    int bbminy = std::min({ a.y, b.y, c.y });
    int bbmaxx = std::max({ a.x, b.x, c.x });
    int bbmaxy = std::max({ a.y, b.y, c.y });
    // get total area of the triangle
    double total_area = signed_triangle_area(a, b, c);
    // if < 1.0, then the face is back facing
    if (total_area < 1.0) return;

#pragma omp parallel for
    for (int x = bbminx; x <= bbmaxx; x++) {
        for (int y = bbminy; y <= bbmaxy; y++) {
            double alpha = signed_triangle_area(vec3i(x, y, 0), b, c) / total_area;
            double beta = signed_triangle_area(vec3i(x, y, 0), c, a) / total_area;
            double gamma = signed_triangle_area(vec3i(x, y, 0), a, b) / total_area;
            if (alpha < 0.0 || beta < 0.0 || gamma < 0.0) continue;
            double alp = alpha, bet = beta, gam = gamma;
            if (alp > bet) std::swap(alp, bet);
            if (bet > gam) std::swap(bet, gam);
            if (alp > bet) std::swap(alp, bet);
            if (alp > 1.0 / 16.0 && bet > 1.0 / 16.0) continue;
            unsigned char az = static_cast<unsigned char>(alpha * 255);
            unsigned char bz = static_cast<unsigned char>(beta * 255);
            unsigned char cz = static_cast<unsigned char>(gamma * 255);
            framebuffer.set(x, y, {az, bz, cz, 0});
        }
    }
}

constexpr int width = 64;
constexpr int height = 64;

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
    Model t = getData("../../../a.txt", width, height);
    //Model t = getData("../../../obj/diablo3_pose/diablo3_pose.obj", width, height);
    //Model t = getData("../../../obj/african_head/african_head.obj", width, height);
    triangle(floatToInt(t.atv(0)), floatToInt(t.atv(1)), floatToInt(t.atv(2)), framebuffer, green);
    //std::srand(std::time({}));

    //for (int i = 0; i < t.nfaces(); i++) {
    //    std::cout << "Row " << i << '\n';
    //    auto [ai, bi, ci] = t.atf(i);
    //    auto a = t.projection(t.atv(ai));
    //    auto b = t.projection(t.atv(bi));
    //    auto c = t.projection(t.atv(ci));
    //    TGAColor color = { static_cast<unsigned char>(rand() % 255),
    //                       static_cast<unsigned char>(rand() % 255),
    //                       static_cast<unsigned char>(rand() % 255),
    //                       static_cast<unsigned char>(rand() % 255) };
    //    triangle(a, b, c, framebuffer, color);
    //}
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

