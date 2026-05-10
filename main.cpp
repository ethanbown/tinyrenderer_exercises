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
constexpr int width = 800;
constexpr int height = 800;

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

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
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


void triangle(const vec3i a, const vec3i b, const vec3i c, TGAImage& framebuffer, const TGAColor color) {
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

void triangleFill(const vec3i a, const vec3i b, const vec3i c, TGAImage& framebuffer, const TGAColor color) {

    // create rectangle that surrounds the triagle
    // with lower left corner and upper right corner
    auto lowerLeftX = std::min({ a.x, b.x, c.x });
    auto lowerLeftY = std::min({ a.y, b.y, c.y });
    auto upperRightX = std::max({ a.x, b.x, c.x });
    auto upperRightY = std::max({ a.y, b.y, c.y });
    // store halfway points of AB, BC, and CA vectors and
    // create vectors orthogonal to AB, BC, and CA
    //std::cout << a << '\n' << b << '\n' << c << "\n\n";

    vec3i ABh(std::floor((a.x + b.x) / 2.0), std::floor((a.y + b.y) / 2.0), 0),
          BCh(std::floor((b.x + c.x) / 2.0), std::floor((b.y + c.y) / 2.0), 0),
          CAh(std::floor((c.x + a.x) / 2.0), std::floor((c.y + a.y) / 2.0), 0);
    auto [ax, ay, az] = b - a;
    auto [bx, by, bz] = c - b;
    auto [cx, cy, cz] = a - c;

    std::swap(ax, ay);
    ax = -ax;
    std::swap(bx, by);
    bx = -bx;
    std::swap(cx, cy);
    cx = -cx;
    vec3i ao(ax, ay, 0), bo(bx, by, 0), co(cx, cy, 0);
    //std::cout << ao << '\n' << bo << '\n' << co << '\n';
    
    // loop over every pixel in the box and check
    // if it is inside the triangle
    for (int i = lowerLeftX; i <= upperRightX; i++) {
        for (int j = lowerLeftY; j <= upperRightY; j++) {
            //std::cout << i << " " << j << '\n';
            // create vector from pixel to
            // midway point of AB, BC, and CA
            vec3i pixelAB = ABh - vec3i(i, j, 0);
            vec3i pixelBC = BCh - vec3i(i, j, 0);
            vec3i pixelCA = CAh - vec3i(i, j, 0);
            // check if all three dot products are
            // the same sign
            int dotao = pixelAB.dot(ao);
            int dotbo = pixelBC.dot(bo);
            int dotco = pixelCA.dot(co);
            //std::cout << dotao << " " << dotbo << " " << dotco << '\n';
            bool allPos = (dotao >= 0 && dotbo >= 0 && dotco >= 0);
            bool allNeg = (dotao <= 0 && dotbo <= 0 && dotco <= 0);
            //std::cout << (allPos || allNeg) << '\n';
            
            // only color pixel if all three dot products are positive
            if (allPos || allNeg) {
                framebuffer.set(i, j, color);
            }
        }
        /*
        framebuffer.set(a.x, a.y, yellow);
        framebuffer.set(b.x, b.y, yellow);
        framebuffer.set(c.x, c.y, yellow);*/
    }
}

int main(int argc, char** argv) {
    TGAImage framebuffer(width, height, TGAImage::RGB);
   /* Model t = getData("../../../a.txt", width, height);
    triangleFill(floatToInt(t.atv(0)), floatToInt(t.atv(1)), floatToInt(t.atv(2)), framebuffer, red);
    triangleFill(floatToInt(t.atv(3)), floatToInt(t.atv(4)), floatToInt(t.atv(5)), framebuffer, white);
    triangleFill(floatToInt(t.atv(6)), floatToInt(t.atv(7)), floatToInt(t.atv(8)), framebuffer, green);
    */
    Model t = getData("../../../obj/african_head/african_head.obj", 800, 800);
    std::srand(std::time({}));

    for (int i = 0; i < t.nfaces(); i++) {
        std::cout << "Row " << i << '\n';
        auto [ai, bi, ci] = t.atf(i);
        auto a = t.projection(t.atv(ai));
        auto b = t.projection(t.atv(bi));
        auto c = t.projection(t.atv(ci));
        TGAColor color = { static_cast<unsigned char>(rand() % 255),
                           static_cast<unsigned char>(rand() % 255),
                           static_cast<unsigned char>(rand() % 255),
                           static_cast<unsigned char>(rand() % 255) };
        triangleFill(a, b, c, framebuffer, color);
    }
    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

