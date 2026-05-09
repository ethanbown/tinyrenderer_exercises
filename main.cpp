#include <cmath>
#include <iostream>
#include <sstream>
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

void line(int ax, int ay, int bx, int by, TGAImage& framebuffer, TGAColor color) {
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
    for (int x = ax; x <= bx; x++) {
        float t = (x - ax) / static_cast<float>(bx - ax);
        int y = std::round(ay + (by - ay) * t);
        if (steep) // if transposed, de-transpose
            framebuffer.set(y, x, color);
        else
            framebuffer.set(x, y, color);
    }
}

Model getData(std::string_view path) {
    std::ifstream in(path.data(), std::ifstream::in);
    if (!in.is_open()) {
        std::cerr << "Failed to open file " << path << '\n';
        return Model();
    }
    Model returnVal;
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

vec3f projection(vec3f pos) {
    auto [xf, yf, zf] = pos;
    return vec3f((xf + 1.0) * width / 2,
                 (yf + 1.0) * width / 2,
                 (zf + 1.0)* width / 2);
}

int main(int argc, char** argv) {
    constexpr float step = 2 / static_cast<float>(width);
    TGAImage framebuffer(width, height, TGAImage::RGB);

    Model data = getData("../../../obj/diablo3_pose/diablo3_pose.obj");

    for (int i = 0; i < data.nfaces(); i++) {
        auto [a, b, c] = data.atf(i);
        auto [ax, ay, az] = projection(data.atv(a));
        auto [bx, by, bz] = projection(data.atv(b));
        auto [cx, cy, cz] = projection(data.atv(c));

        line(ax, ay, bx, by, framebuffer, red);
        line(cx, cy, bx, by, framebuffer, red);
        line(cx, cy, ax, ay, framebuffer, red);

        framebuffer.set(ax, ay, white);
        framebuffer.set(bx, by, white);
        framebuffer.set(cx, cy, white);
    }

    framebuffer.write_tga_file("framebuffer.tga");
    return 0;
}

