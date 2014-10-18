#pragma once

#include "glHeaders.hpp"
#include "shader.hpp"
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace glm;

class Voxel{
public:
	bool fill=false;
};

class VoxelGrid{
private:
	float voxel_size = 2; // 1 cell per 10 units

	Voxel *grid;
	vec3 bottom_left, top_right;

	int x_res, y_res, z_res;

	GridShader grid_shader;

public:
	VoxelGrid(vec3 _bottom_left, vec3 _top_right);
	~VoxelGrid();

	void render(mat4 projection, mat4 view, mat4 model);
	void render_cast(mat4 projection, mat4 view, mat4 model, float w, float h, vec3 light);

	void reset();
	void add(vec3 pos);

};
