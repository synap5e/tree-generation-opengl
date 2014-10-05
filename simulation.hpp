#pragma once

//#include "node.hpp"
//#include "stem.hpp"
#include "render.hpp"
#include "shader.hpp"
#include "tree.hpp"

#include "mesh.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>

class Simulation
{
private:
	float xrot = 90;
	float yrot = 0;
	float radius = 300;

	Tree *tree;

	BranchShader branch_shader;
	AttractionPointShader point_shader;

public:
	Simulation();
	void initialize();
	void simulation_step(float dtSeconds);
	void render(int, int);

	void key_handler(int key, int scancode, int action, int mods);
	void mouse_drag(double x, double y);

};
