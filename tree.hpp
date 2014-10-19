#pragma once

#include "branch.hpp"
#include "shader.hpp"
#include "voxels.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <set>
#include <atomic>
#include <mutex>
#include <picojson.h>
#include <muParser.h>

using namespace glm;

/*struct vec3_comparator{
	bool operator()(const vec3 &vecA, const vec3 &vecB){ 
		const double epsilion = 0.01;
		return  	fabs(vecA[0] -vecB[0]) < epsilion   
		      	&& 	fabs(vecA[1] -vecB[1]) < epsilion   
		      	&& 	fabs(vecA[2] -vecB[2]) < epsilion;
	} 
};*/

typedef struct VertexLists{
    std::vector<vec3> branch_verts;
    std::vector<float> branch_radii;
    std::vector<unsigned int> branch_indexes;

    std::vector<vec3> leaf_locations;
    std::vector<mat3> leaf_rotations;
    std::vector<float> leaf_scales;
} VertexLists;

class TreeParams{
public:
	float radius;
	float height;
	float root_height;
	float canopy_exponent;

	int initial_attraction_points;
	int attraction_point_creation_rate;
	float branch_length;

	// actual distance is * branch_length
	float kill_distance;
	float influence_distance;

	// how much wieght is given to the branches existing direction
	// as opposed to the pull from the attraction points
	float soft_bends_weight;

	int branch_kill_age;

};

class Tree{
private:
	vec3 position;

	Branch *root;
	int simulation_time = 0;
	int seed_timer;


    std::vector<vec3> branch_locations;

	std::vector<AttractionPoint*> attraction_points;
    std::vector<Branch*> branches;

	std::vector<Branch*> live_branches;    

	void generate_crown(int count);
	void generate_trunk();


	int ageof(Branch* b);


	typedef struct BranchRadiusVariables{
		double descendants;
		double age;
		double depth;
	} BranchRadiusVariables;
	BranchRadiusVariables brach_radius_variables;

	mu::Parser branch_radius_parser;
	mu::Parser leaf_check_parser;
	mu::Parser leaf_size_parser;
	
	void init_branch_parser(mu::Parser &parser, picojson::object function_params);


public:

	TreeParams params;
	VertexLists vertex_lists;

	Tree(vec3 pos, picojson::object tree_params, int _seed_timer);
	~Tree();
	bool grow();
	void regenerate_vertex_lists();

	void update(VoxelGrid *grid, vec3 light);

	std::vector<AttractionPoint*> get_attraction_points();

};
