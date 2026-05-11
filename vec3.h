#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>


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

	T dot(const vec3<T>& vec) {
		return this->x * vec.x 
			 + this->y * vec.y
			 + this->z * vec.z;
	}

	T dot(const vec3<T>& vec) const {
		return this->x * vec.x
			 + this->y * vec.y
			 + this->z * vec.z;
	}

	vec3<T> operator+(const vec3<T>& vec) {
		return vec3<T>(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	}

	vec3<T> operator+(const vec3<T>& vec) const {
		return vec3<T>(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	}

	vec3<T> operator+(const T val) {
		return vec3<T>(this->x + val, this->y + val, this->z + val);
	}

	vec3<T> operator+(const T val) const {
		return vec3<T>(this->x + val, this->y + val, this->z + val);
	}

	vec3<T> operator-(const vec3<T>& vec) {
		return vec3<T>(this->x - vec.x, this->y - vec.y, this->z - vec.z);
	}

	vec3<T> operator-(const vec3<T>& vec) const {
		return vec3<T>(this->x - vec.x, this->y - vec.y, this->z - vec.z);
	}

	vec3<T> operator-(const T val) {
		return vec3<T>(this->x - val, this->y - val, this->z - val);
	}

	vec3<T> operator-(const T val) const {
		return vec3<T>(this->x - val, this->y - val, this->z - val);
	}

	vec3<T> operator*(const T val) {
		return vec3<T>(this->x * val, this->y * val, this->z * val);
	}

	vec3<T> operator*(const T val) const {
		return vec3<T>(this->x * val, this->y * val, this->z * val);
	}

	vec3<T> operator/(const T val) {
		return vec3<T>(this->x / val, this->y / val, this->z / val);
	}

	vec3<T> operator/(const T val) const {
		return vec3<T>(this->x / val, this->y / val, this->z / val);
	}

	void static swap(vec3<T>& v1, vec3<T>& v2) {
		std::swap(v1.x, v2.x);
		std::swap(v1.y, v2.y);
		std::swap(v1.z, v2.z);
	}

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const vec3<T>& vec);
};

using vec3f = vec3<float>;
using vec3i = vec3<int>;

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec3<T>& vec) {
	os << vec.x << " " << vec.y << " " << vec.z;
	return os;
}

#endif