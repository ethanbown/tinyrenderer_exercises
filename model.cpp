#include "model.h"

vec3i Model::projection(vec3f pos) {
	auto [xf, yf, zf] = pos;
	return vec3i((xf + 1.0f) * this->width / 2,
		(yf + 1.0f) * this->height / 2,
		(zf + 1.0f) * this->width / 2);
}

size_t Model::nvertices() const {
	return this->vertices.size();
}

size_t Model::nfaces() const {
	return this->faces.size();
}

void Model::addv(float x, float y, float z) {
	this->vertices.push_back(vec3(x, y, z));
}

void Model::addf(int x, int y, int z) {
	this->faces.push_back(vec3i(x, y, z));
}

vec3f& Model::atv(const int i) {
	return this->vertices.at(i);
}

vec3f Model::atv(const int i) const {
	return this->vertices.at(i);
}

vec3i& Model::atf(const int i) {
	return this->faces.at(i);
}

vec3i Model::atf(const int i) const {
	return this->faces.at(i);
}