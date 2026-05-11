#ifndef VEC4_H
#define VEC4_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

template<typename T>
struct vec4 {
	T x, y, z, w;

	vec4() : x(T{}), y(T{}), z(T{}), w(T{}) {}
	vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

	T& operator[](const int i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}

	T operator[](const int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		case 2: return z;
		case 3: return w;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}

	T dot(const vec4<T>& vec) {
		return this->x * vec.x 
			 + this->y * vec.y
			 + this->z * vec.z
			 + this->w * vec.w;
	}

	T dot(const vec4<T>& vec) const {
		return this->x * vec.x
			 + this->y * vec.y
			 + this->z * vec.z
			 + this->w * vec.w;
	}

	vec4<T> operator+(const vec4<T>& vec) {
		return vec4<T>(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
	}

	vec4<T> operator+(const vec4<T>& vec) const {
		return vec4<T>(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
	}

	vec4<T> operator+(const T val) {
		return vec4<T>(this->x + val, this->y + val, this->z + val, this->w + val);
	}

	vec4<T> operator+(const T val) const {
		return vec4<T>(this->x + val, this->y + val, this->z + val, this->w + val);
	}

	vec4<T> operator-(const vec4<T>& vec) {
		return vec4<T>(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
	}

	vec4<T> operator-(const vec4<T>& vec) const {
		return vec4<T>(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
	}

	vec4<T> operator-(const T val) {
		return vec4<T>(this->x - val, this->y - val, this->z - val, this->w - val);
	}

	vec4<T> operator-(const T val) const {
		return vec4<T>(this->x - val, this->y - val, this->z - val, this->w - val);
	}

	vec4<T> operator*(const T val) {
		return vec4<T>(this->x * val, this->y * val, this->z * val, this->w * val);
	}

	vec4<T> operator*(const T val) const {
		return vec4<T>(this->x * val, this->y * val, this->z * val, this->w * val);
	}

	vec4<T> operator/(const T val) {
		return vec4<T>(this->x / val, this->y / val, this->z / val, this->w / val);
	}

	vec4<T> operator/(const T val) const {
		return vec4<T>(this->x / val, this->y / val, this->z / val, this->w / val);
	}

	void static swap(vec4<T>& v1, vec4<T>& v2) {
		std::swap(v1.x, v2.x);
		std::swap(v1.y, v2.y);
		std::swap(v1.z, v2.z);
		std::swap(v1.w, v2.w);
	}

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const vec4<T>& vec);
};

using vec4f = vec4<float>;
using vec4i = vec4<int>;

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec4<T>& vec) {
	os << vec.x << " " << vec.y << " " << vec.z << " " << vec.w;
	return os;
}

#endif