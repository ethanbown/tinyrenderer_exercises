#ifndef MATRIX_H
#define MATRIX_H
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>

template<typename T>
T dot(const std::vector<T>& row, const std::vector<T>& col);

template <typename T, size_t X, size_t Y>
class mat {
	std::vector<std::vector<T>> data;

public:

	mat() {
		data.resize(X, std::vector<T>(Y));
	}

	mat(const T val) {
		data.resize(X, std::vector<T>(Y, val));
	}

	mat(const std::vector<std::vector<T>>& values) {
		data = values;
	}

	mat<T, X, X> static identity() {
		mat<T, X, X> newMat;
		for (int i = 0; i < X; i++) {
			newMat[i][i] = T(1);
		}
		return newMat;
	}

	mat<T, X, Y> operator+(const mat<T, X, Y>& rhs) const {
		mat<T, X, Y> newMat;
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				newMat[i][j] = this->data[i][j] + rhs[i][j];
			}
		}
		return newMat;
	}

	mat<T, X, Y> operator-(const mat<T, X, Y>& rhs) const {
		mat<T, X, Y> newMat;
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Y; j++) {
				newMat[i][j] = this->data[i][j] - rhs[i][j];
			}
		}
		return newMat;
	}

	std::vector<T>& operator[](const int i) {
		return this->data[i];
	}

	const std::vector<T>& operator[](const int i) const {
		return this->data[i];
	}

	template <size_t Z>
	mat<T, X, Z> operator*(const mat<T, Y, Z>& rhs) const {
		mat<T, X, Z> newMat;
		for (int i = 0; i < X; i++) {
			for (int j = 0; j < Z; j++) {
				newMat[i][j] = dot(this->data[i], rhs.colToVec(j));
			}
		}
		return newMat;
	}

	vec3<T> operator*(const vec3<T>& rhs) const {
		mat<T, 3, 1> temp;
		for (int i = 0; i < 3; i++) {
			temp[i][0] = rhs[i];
		}
		mat<T, 3, 1> temp2 = *this * temp;
		vec3<T> res;
		res.x = temp2[0][0];
		res.y = temp2[1][0];
		res.z = temp2[2][0];
		return res;
	}

	std::vector<T> colToVec(const int j) const {
		std::vector<T> newVec(X);
		for (int i = 0; i < X; i++) {
			newVec[i] = this->data[i][j];
		}
		return newVec;
	}

	template <typename T, size_t X, size_t Y>
	friend std::ostream& operator<<(std::ostream& os, const mat<T, X, Y>& rhs);
};

template<typename T>
T dot(const std::vector<T>& row, const std::vector<T>& col) {
	T sum = 0;
	for (int i = 0; i < row.size(); i++) {
		sum += row[i] * col[i];
	}
	if (std::abs(sum) < 1e-7) {
		return T(0);
	}
	return sum;
}

template <typename T, size_t X, size_t Y>
mat<T, Y, X> transpose(const mat<T, X, Y>& input) {
	mat<T, Y, X> newMat;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < Y; j++) {
			newMat[j][i] = input[i][j];
		}
	}
	return newMat;
}

template <typename T, size_t X>
mat<T, X, X> invert(const mat<T, X, X>& val) {
	// Need to build augmented matrix [val | identity matrix]
	mat<T, X, X * 2> augment;
	for (int i = 0; i < X; i++) {
		for (int j = 0; j < X; j++) {
			augment[i][j] = val[i][j];
		}
		augment[i][X + i] = T(1);
	}

	// actual inversion
	for (int col = 0; col < X; col++) {
		// need to find row with largest magintude in this column
		int pivot = col;
		for (int row = col + 1; row < X; row++) {
			if (std::abs(augment[row][col]) > std::abs(augment[pivot][col])) {
				pivot = row;
			}
		}

		if (std::abs(augment[pivot][col]) < T(1e-9)) {
			throw std::runtime_error("Matrix is singular, cannot invert");
		}
		std::swap(augment[pivot], augment[col]);

		// Scale pivot row so that diagonal becomes 1
		T scale = augment[col][col];
		for (int j = 0; j < 2 * X; j++) {
			augment[col][j] /= scale;
		}

		// eliminate this column in all other rows
		for (int row = 0; row < X; row++) {
			if (row == col) continue;
			T factor = augment[row][col];
			for (int j = 0; j < X * 2; j++) {
				augment[row][j] -= factor * augment[col][j];
			}
		}
	}

	// Get out right half
	mat<T, X, X> res;
	for (int i = 0; i < X; i++) {
		for (int j = X; j < 2 * X; j++) {
			res[i][j - X] = augment[i][j];
		}
	}
	return res;
}

template <typename T, size_t X, size_t Y>
std::ostream& operator<<(std::ostream& os, const mat<T, X, Y>& rhs) {
	for (int i = 0; i < X; i++) {
		os << "[";
		int j = 0;
		for (; j + 1 < Y; j++) {
			os << rhs[i][j] << ",";
		}
		os << rhs[i][j] << "]\n";
	}
	return os;
}

using mat2i = mat<int, 2, 2>;
using mat3i = mat<int, 3, 3>;
using mat4i = mat<int, 4, 4>;

using mat2f = mat<float, 2, 2>;
using mat3f = mat<float, 3, 3>;
using mat4f = mat<float, 4, 4>;

#endif