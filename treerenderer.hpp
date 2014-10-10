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

	typedef struct ElementGroup{
		GLuint element_buffer;
		int size = 0;
	} ElementGroup;

	Tree* tree;
	BranchShader branch_shader;
	AttractionPointShader point_shader;

    
    GLuint vertex_vbo; 
    GLuint size_vbo;
    ElementGroup branch_elements;
    ElementGroup leaf_elements;
    
public:
	TreeRenderer(Tree* _tree);

	void regenerate();
	void render(glm::mat4 projection, glm::mat4 view);

};
