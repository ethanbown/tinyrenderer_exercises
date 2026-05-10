#include <vector>
#include "vec3.h"

class Model {
    int width, height;
    std::vector<vec3f> vertices;
    std::vector<vec3i> faces;
public:
    Model() : width(64), height(64) {};
    Model(const int width, const int height) : width(width), height(height) {};
    size_t nvertices() const;
    size_t nfaces() const;
    vec3i projection(vec3f pos);
    void addv(float x, float y, float z);
    void addf(int x, int y, int z);
    vec3f& atv(const int i);
    vec3i& atf(const int i);
    vec3f atv(const int i) const;
    vec3i atf(const int i) const;
};