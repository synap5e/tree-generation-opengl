#include "voxels.hpp"
#include <vector>

VoxelGrid::VoxelGrid(vec3 _bottom_left, vec3 _top_right): bottom_left(_bottom_left), top_right(_top_right){
	grid_shader.load();

	x_res = (top_right.x -  bottom_left.x) / voxel_size;
	y_res = (top_right.y -  bottom_left.y) / voxel_size;
	z_res = (top_right.z -  bottom_left.z) / voxel_size;

	grid = new Voxel[x_res * y_res * z_res];
}

VoxelGrid::~VoxelGrid(){
	delete grid;
}

void VoxelGrid::add(vec3 pos){
	float x = (pos.x - bottom_left.x)/voxel_size;
	float y = (pos.y - bottom_left.y)/voxel_size;
	float z = (pos.z - bottom_left.z)/voxel_size;

	if (0 < x && x < x_res && 0 < y && y < y_res && 0 < z && z < z_res){
		int ind = (int(x) * x_res + int(y)) * y_res + int(z);
		grid[ind].fill = true;
	}

	//printf("%d, %d, %d\n", int(x), int(y), int(z));
}

void VoxelGrid::reset(){
	for (int i=0; i < x_res * y_res * z_res; ++i){
		grid[i].fill = false;
	}
}

void add_cube(std::vector<vec3> &v, vec3 p, float s){
	// front face
	v.push_back(p + vec3(0, 0, 0));
	v.push_back(p + vec3(0, s, 0));
	v.push_back(p + vec3(s, 0, 0));

	v.push_back(p + vec3(0, s, 0));
	v.push_back(p + vec3(s, s, 0));
	v.push_back(p + vec3(s, 0, 0));

	// back face
	v.push_back(p + vec3(0, 0, s));
	v.push_back(p + vec3(0, s, s));
	v.push_back(p + vec3(s, 0, s));

	v.push_back(p + vec3(0, s, s));
	v.push_back(p + vec3(s, s, s));
	v.push_back(p + vec3(s, 0, s));

	// left face
	v.push_back(p + vec3(0, 0, 0));
	v.push_back(p + vec3(0, 0, s));
	v.push_back(p + vec3(0, s, s));

	v.push_back(p + vec3(0, 0, 0));
	v.push_back(p + vec3(0, s, 0));
	v.push_back(p + vec3(0, s, s));

	// right face
	v.push_back(p + vec3(s, 0, 0));
	v.push_back(p + vec3(s, 0, s));
	v.push_back(p + vec3(s, s, s));

	v.push_back(p + vec3(s, 0, 0));
	v.push_back(p + vec3(s, s, 0));
	v.push_back(p + vec3(s, s, s));

	// bottom face
	v.push_back(p + vec3(0, 0, 0));
	v.push_back(p + vec3(0, 0, s));
	v.push_back(p + vec3(s, 0, s));

	v.push_back(p + vec3(0, 0, 0));
	v.push_back(p + vec3(s, 0, s));
	v.push_back(p + vec3(s, 0, 0));

	// top face
	v.push_back(p + vec3(0, s, 0));
	v.push_back(p + vec3(0, s, s));
	v.push_back(p + vec3(s, s, s));

	v.push_back(p + vec3(0, s, 0));
	v.push_back(p + vec3(s, s, s));
	v.push_back(p + vec3(s, s, 0));

}

void VoxelGrid::render(mat4 projection, mat4 view, mat4 model){
	std::vector<vec3> verts;
	for (int x=0; x<x_res; ++x){
		float x_pos = bottom_left.x + x * voxel_size;
		for (int y=0; y<y_res; ++y){
			float y_pos = bottom_left.y + y * voxel_size;
			//printf("%f\n", y_pos);

			for (int z=0; z<z_res; ++z){
				int ind = (x * x_res + y) * y_res + z;
				if (!grid[ind].fill) continue;

				float z_pos = bottom_left.z + z * voxel_size;

				vec3 b(x_pos, y_pos, z_pos);

				add_cube(verts, b, voxel_size);

			}
		}
	}

	grid_shader.activate();
	grid_shader.set_model(model);
	grid_shader.set_view(view);
	grid_shader.set_projection(projection);

	GLuint vb;
	glGenBuffers(1, &vb);
    glBindBuffer(GL_ARRAY_BUFFER, vb);
    glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(vec3), &verts[0], GL_STATIC_DRAW);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnable(GL_BLEND);
    glDisable(GL_CULL_FACE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDrawArrays (GL_TRIANGLES, 0, verts.size());
    glDisable(GL_BLEND);
    glEnable(GL_CULL_FACE);

    glDeleteBuffers(1, &vb);
}


VoxelGrid::render_cast(mat4 projection, mat4 view, mat4 model, float w, float h, vec3 light){

}
