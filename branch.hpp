#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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

	int index;

	Branch(Branch *_parent, vec3 _pos, vec3 _grow): parent(_parent), position(_pos), grow_direction(_grow), original_grow_direction(_grow){};

};
