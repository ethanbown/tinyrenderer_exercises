#include <vector>
#include "vec3.h"

class Model {
    std::vector<vec3f> vertices;
    std::vector<vec3i> faces;
public:
    size_t nvertices() const;
    size_t nfaces() const;
    void addv(float x, float y, float z);
    void addf(int x, int y, int z);
    vec3f& atv(const int i);
    vec3i& atf(const int i);
    vec3f atv(const int i) const;
    vec3i atf(const int i) const;
};