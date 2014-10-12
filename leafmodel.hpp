#pragma once

#include "glHeaders.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <map>

using namespace glm;

class LeafModel{
private:
	
public:
	GLuint vao = 0;
	GLuint points_vbo;
	GLuint colours_vbo;
	GLuint normals_vbo;
	int size;
	LeafModel();
	void bind();
};
