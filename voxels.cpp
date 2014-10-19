#include "voxels.hpp"
#include <vector>

VoxelGrid::VoxelGrid(vec3 _bottom_left, vec3 _top_right): bottom_left(_bottom_left), top_right(_top_right){
	grid_shader.load();
	shadow_shader.load();

	x_res = (top_right.x -  bottom_left.x) / voxel_size;
	y_res = (top_right.y -  bottom_left.y) / voxel_size;
	z_res = (top_right.z -  bottom_left.z) / voxel_size;

	grid = new Voxel[x_res * y_res * z_res];
}

VoxelGrid::~VoxelGrid(){
	delete grid;
}

void VoxelGrid::add(vec3 pos){
	if (pos.x > top_right.x || pos.x < bottom_left.x || pos.y > top_right.y || pos.y < bottom_left.y || pos.z > top_right.z || pos.z < bottom_left.z){
		return;
	}

	float x = (pos.x - bottom_left.x)/voxel_size;
	float y = (pos.y - bottom_left.y)/voxel_size;
	float z = (pos.z - bottom_left.z)/voxel_size;

	if (0 < x && x < x_res && 0 < y && y < y_res && 0 < z && z < z_res){
		int ind = (int(x) * x_res + int(y)) * y_res + int(z);
		grid[ind].fill++;
		//if (int(x) == 0)
		//	printf("set (%d, %d, %d) = %d\n", int(x), int(y), int(z), grid[ind].fill);
	}

	//printf("%d, %d, %d\n", int(x), int(y), int(z));
}

void VoxelGrid::reset(){
	for (int i=0; i < x_res * y_res * z_res; ++i){
		grid[i].fill = 0;
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

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

float intbound(float s, float ds) {
	if (ds < 0) {
		return intbound(-s, -ds);
	} else {
		s = fmod(s, 1);
		return (1-s)/ds;
	}
}

// From "A Fast Voxel Traversal Algorithm for Ray Tracing"
// by John Amanatides and Andrew Woo, 1987
int VoxelGrid::cast(vec3 origin, vec3 direction){
	int iscts = 0;

	float x = origin.x;
	float y = origin.y;
	float z = origin.z;

	// Direction to increment x,y,z when stepping.
	float stepX = sgn(direction.x);
	float stepY = sgn(direction.y);
	float stepZ = sgn(direction.z);

	// The initial values depend on the fractional part of the origin.
	float tMaxX = intbound(origin.x, direction.x);
	float tMaxY = intbound(origin.y, direction.y);
	float tMaxZ = intbound(origin.z, direction.z);

	// The change in t when taking a step
	float tDeltaX = stepX/direction.x;
	float tDeltaY = stepY/direction.y;
	float tDeltaZ = stepZ/direction.z;

	// while ray has not exited world (but may not have entered)
	while ( (stepX > 0 ? x < top_right.x : x >= bottom_left.x) &&
			(stepY > 0 ? y < top_right.y : y >= bottom_left.y) &&
			(stepZ > 0 ? z < top_right.z : z >= bottom_left.z)) {
				     

		// Invoke the callback, unless we are not *yet* within the bounds of the
		// world.
		if (!(x < bottom_left.x || y < bottom_left.y || z < bottom_left.z || x >= top_right.x || y >= top_right.y || z >= top_right.z)){
			float x_i = (x - bottom_left.x)/voxel_size;
			float y_i = (y - bottom_left.y)/voxel_size;
			float z_i = (z - bottom_left.z)/voxel_size;
			int ind = (int(x_i) * x_res + int(y_i)) * y_res + int(z_i);
			//printf("through (%f, %f, %f) = %d\n", x, y, z, grid[ind].fill);
			iscts += grid[ind].fill;
		}
		
		// tMaxX stores the t-value at which we cross a cube boundary along the
		// X axis, and similarly for Y and Z. Therefore, choosing the least tMax
		// chooses the closest cube boundary. Only the first case of the four
		// has been commented in detail.
		if (tMaxX < tMaxY) {
			if (tMaxX < tMaxZ) {
				//if (tMaxX > radius) break;
				// Update which cube we are now in.
				x += stepX;
				// Adjust tMaxX to the next X-oriented boundary crossing.
				tMaxX += tDeltaX;
			} else {
				//if (tMaxZ > radius) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
			}
		} else {
			if (tMaxY < tMaxZ) {
				//if (tMaxY > radius) break;
				y += stepY;
				tMaxY += tDeltaY;
			} else {
				// Identical to the second case, repeated for simplicity in
				// the conditionals.
				//if (tMaxZ > radius) break;
				z += stepZ;
				tMaxZ += tDeltaZ;
			}
		}
	}

	return iscts;
}
