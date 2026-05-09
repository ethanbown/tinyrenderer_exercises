#include <stdexcept>
#include <string>


template<typename T>
struct vec3 {
	T x, y, z;

	vec3() : x(T{}), y(T{}), z(T{}) {}
	vec3(T x, T y, T z) : x(x), y(y), z(z) {}

	T& operator[](const int i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}

	T operator[](const int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}
};

using vec3f = vec3<float>;
using vec3i = vec3<int>;