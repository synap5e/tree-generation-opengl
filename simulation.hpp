#pragma once

#include "mesh.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Simulation
{
	void setUniform( const char *name, const glm::mat4 & m);
	void setUniform( const char *name, const glm::mat3 & m);

public:
	Simulation();
	void Initialize();
	void KeyHandler(int key, int scancode, int action, int mods);
	void SimulationStep(float dtSeconds);
	void Render(int, int);
	void DrawMesh(Mesh& mesh,  glm::mat4& V, glm::mat4& P);

	Mesh mesh;

	GLuint m_standardShader;
};
