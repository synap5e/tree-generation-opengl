#pragma once

#include "branch.hpp"
#include "shader.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <map>
#include <set>

using namespace glm;

struct vec3_comparator{
	bool operator()(const vec3 &vecA, const vec3 &vecB){ 
		const double epsilion = 0.01;
		return  	fabs(vecA[0] -vecB[0]) < epsilion   
		      	&& 	fabs(vecA[1] -vecB[1]) < epsilion   
		      	&& 	fabs(vecA[2] -vecB[2]) < epsilion;
	} 
};


class Tree{
private:
	vec3 position;

/*	int LeafCount = 400;
    int TreeWidth = 80;    
    int TreeHeight = 150;   
    int TrunkHeight = 40;
    int MinDistance = 2;
    int MaxDistance = 15;
    int BranchLength = 2;*/

    Branch *root;
    std::vector<Leaf*> leaves;
    std::vector<Branch*> branches;

    std::vector<vec3> branch_locations;

public:
	Tree();

	void generate_crown();

	void generate_trunk();

	void grow();

	void draw(BranchShader bs, LeafShader ls);

	void render(std::vector<vec3> verts, std::vector<float> sizes, GLenum type);


};
