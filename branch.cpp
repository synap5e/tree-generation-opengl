#include "branch.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#define _USE_MATH_DEFINES
#include <math.h>
#include "random.hpp"

Branch::Branch(Branch *_parent, vec3 _pos, vec3 _grow, int _born): parent(_parent), position(_pos), grow_direction(_grow), original_grow_direction(_grow), born(_born){
	if (parent){
		depth = parent->depth+1;
	}


	// http://math.stackexchange.com/questions/442418/random-generation-of-rotation-matrices/442423#442423
	float u1 = RandomGen::get(0, 1);
	float u2 = RandomGen::get(0, 1);

	float theta = acos(2*u1 -1);
	float phi 	= 2*M_PI*u2;

	vec3 random_rotation_axis = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
	float random_rotation = RandomGen::get(0, 2*M_PI);

	rotation = mat3_cast(angleAxis(random_rotation, random_rotation_axis));

};
