#pragma once

#include "branch.hpp"
#include "shader.hpp"

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
	float radius 				= 100.f;
	float height 				= 200.f;
	float root_height 			= 5.f;
	float canopy_exponent 		= 1.01f;

	int attraction_point_count 	= 10000;
	float branch_length 		= 1.f;

	// actual distance is * branch_length
	float kill_distance 		= 1;
	float influence_distance 	= 10;

/*	float initial_radius 		= 0.001;
	float radius_growth 		= 0.002;*/

	// The maximum number of descendants a branch can have
	// to be considered a twig and grow leaves
	float twig_max_descendants = 0;

	// how much wieght is given to the branches existing direction
	// as opposed to the pull from the attraction points
	float soft_bends_weight = 2;

	int branch_kill_age = 10;

};

class Tree{
private:
	vec3 position;

	Branch *root;
	int simulation_time = 0;


    std::vector<vec3> branch_locations;

	std::vector<AttractionPoint*> attraction_points;
    std::vector<Branch*> branches;

	std::vector<Branch*> live_branches;    

	void generate_crown();
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

	Tree();
	Tree(picojson::object tree_params);
	~Tree();
	bool grow();
	void regenerate_vertex_lists();

	std::vector<AttractionPoint*> get_attraction_points();

};
