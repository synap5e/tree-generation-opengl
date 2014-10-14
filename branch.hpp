#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>
#include "random.hpp"

using namespace glm;

class Branch;

class AttractionPoint {
public:
	Branch* closest = nullptr;

	vec3 position;
	AttractionPoint(vec3 _pos): position(_pos){};
};

class Branch{
public:
	Branch* parent;
	vec3 grow_direction;
	vec3 original_grow_direction;
	int grow_count = 0;
	vec3 position;
	float radius;
	int lifespan=10;

	vec3 random_rotation_axis;
	float random_rotation;

	int index;

	Branch(Branch *_parent, vec3 _pos, vec3 _grow): parent(_parent), position(_pos), grow_direction(_grow), original_grow_direction(_grow){
		// http://math.stackexchange.com/questions/442418/random-generation-of-rotation-matrices/442423#442423
		float u1 = RandomGen::get(0, 1);
		float u2 = RandomGen::get(0, 1);

		float theta = acos(2*u1 -1);
		float phi 	= 2*M_PI*u2;

		random_rotation_axis = vec3(sin(theta)*cos(phi), sin(theta)*sin(phi), cos(theta));
		random_rotation = RandomGen::get(0, 2*M_PI);

	};

};
