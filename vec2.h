#ifndef VEC2_H
#define VEC2_H

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>


template<typename T>
struct vec2 {
	T x, y;

	vec2() : x(T{}), y(T{}) {}
	vec2(T x, T y) : x(x), y(y) {}

	T& operator[](const int i) {
		switch (i) {
		case 0: return x;
		case 1: return y;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}

	T operator[](const int i) const {
		switch (i) {
		case 0: return x;
		case 1: return y;
		default: throw std::out_of_range(std::to_string(i) + " is out of range\n");
		}
	}

	T dot(const vec2<T>& vec) {
		return this->x * vec.x
			+ this->y * vec.y;
	}

	T dot(const vec2<T>& vec) const {
		return this->x * vec.x
			+ this->y * vec.y;
	}

	vec2<T> operator+(const vec2<T>& vec) {
		return vec2<T>(this->x + vec.x, this->y + vec.y);
	}

	vec2<T> operator+(const vec2<T>& vec) const {
		return vec2<T>(this->x + vec.x, this->y + vec.y);
	}

	vec2<T> operator+(const T val) {
		return vec2<T>(this->x + val, this->y + val);
	}

	vec2<T> operator+(const T val) const {
		return vec2<T>(this->x + val, this->y + val);
	}

	vec2<T> operator-(const vec2<T>& vec) {
		return vec2<T>(this->x - vec.x, this->y - vec.y);
	}

	vec2<T> operator-(const vec2<T>& vec) const {
		return vec2<T>(this->x - vec.x, this->y - vec.y);
	}

	vec2<T> operator-(const T val) {
		return vec2<T>(this->x - val, this->y - val);
	}

	vec2<T> operator-(const T val) const {
		return vec2<T>(this->x - val, this->y - val);
	}

	vec2<T> operator*(const T val) {
		return vec2<T>(this->x * val, this->y * val);
	}

	vec2<T> operator*(const T val) const {
		return vec2<T>(this->x * val, this->y * val);
	}

	vec2<T> operator/(const T val) {
		return vec2<T>(this->x / val, this->y / val);
	}

	vec2<T> operator/(const T val) const {
		return vec2<T>(this->x / val, this->y / val);
	}

	void static swap(vec2<T>& v1, vec2<T>& v2) {
		std::swap(v1.x, v2.x);
		std::swap(v1.y, v2.y);
	}

	template<typename T>
	friend std::ostream& operator<<(std::ostream& os, const vec2<T>& vec);
};

using vec2f = vec2<float>;
using vec2i = vec2<int>;

template <typename T>
std::ostream& operator<<(std::ostream& os, const vec2<T>& vec) {
	os << vec.x << " " << vec.y;
	return os;
}

#endif