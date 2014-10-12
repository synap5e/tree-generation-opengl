#pragma once

#include "glHeaders.hpp"

class LeafModel{
private:
	
public:
	GLuint vao = 0;
	GLuint points_vbo;
	GLuint colours_vbo;
	int size;
	LeafModel();
	void bind();
};
