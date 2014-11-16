#pragma once

#include "glHeaders.hpp"
#include "shader.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace glm;

class Voxel{
public:
	int fill=0;
};

class VoxelGrid{
private:
	float voxel_size = 10;

	Voxel *grid;
	vec3 bottom_left, top_right;

	int x_res;
	int y_res;
	int z_res;

	GridShader grid_shader;

public:
	VoxelGrid(vec3 _bottom_left, vec3 _top_right);
	~VoxelGrid();

	void render(mat4 &projection, mat4 &view, mat4 &model);
	//void render_cast(mat4 projection, mat4 view, mat4 model, vec3 light, Tree *tree);

	int cast(vec3 origin, vec3 direction);

	void reset();
	void add(vec3 pos);

};
