#pragma once  

#include "glHeaders.hpp"
#include "tree.hpp"
#include "leafmodel.hpp"
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
	LeafModel leaf_model;

	static BranchShader branch_shader;
	static AttractionPointShader point_shader;
	static LeafShader leaf_shader;
	static GridShader grid_shader;
	static bool shaders_loaded;
    
    GLuint vertex_vbo; 
    GLuint size_vbo;
    GLuint depth_vbo;
    ElementGroup branch_elements;

    ElementGroup leaf_elements;
    GLuint leaf_rotations_vbo;
    GLuint leaf_scales_vbo;

public:
	TreeRenderer(Tree* _tree);

	bool draw_attraction_points = false;

	void regenerate();
	void render(glm::mat4 &projection, glm::mat4 &view, glm::mat4 &model, glm::vec3 &light);

};