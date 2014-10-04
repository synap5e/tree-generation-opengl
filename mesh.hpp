#pragma once

#include "glHeaders.hpp"

#include <vector>
#include <glm/glm.hpp>

class Mesh
{
private:
	GLuint vertex_vbo;
	GLuint colors_vbo;
	GLuint normals_vbo;

public:
    Mesh(float height, float radius);
    ~Mesh();
    //void create(float height, float radius);
 
    GLuint vao;
    GLuint size;

};
