#pragma once  

#include "tree.hpp"
#include "shader.hpp"

#include <vector>
#include <map>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>


class TreeRenderer{
private:
	Tree* tree;
	BranchShader branch_shader;
	AttractionPointShader point_shader;
public:
	TreeRenderer(Tree* _tree);

	void render(glm::mat4 projection, glm::mat4 view);

};
