#include "model.h"

size_t Model::nvertices() const {
	return this->vertices.size();
}

size_t Model::nfaces() const {
	return this->faces.size();
}

void Model::addv(float x, float y, float z) {
	this->vertices.push_back(vec3f(x, y, z));
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