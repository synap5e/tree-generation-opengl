#pragma once

#include "glHeaders.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <vector>
#include <map>

using namespace glm;

class LeafModel{
private:

	float PI = 3.141592653589793f;
	int x_steps = 20;
	int z_steps = 6;
	float scale = 0.5;

	
public:
	GLuint vao = 0;
	GLuint points_vbo;
	GLuint colours_vbo;
	GLuint normals_vbo;
	int size;
	LeafModel();
	void bind();
};
