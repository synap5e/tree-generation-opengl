#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

using namespace glm;

class Branch;

class AttractionPoint {
public:
	Branch* closest = nullptr;

	float weight = 1;
	vec3 position;
	AttractionPoint(vec3 _pos): position(_pos){};
};

class Branch{
public:
	Branch* parent;
	float grow_count = 0;
	vec3 position;
	
	int born;
	int descendants = 0;
	int depth = 0;

	vec3 grow_direction;
	vec3 original_grow_direction;

	mat3 rotation;
	int index;

	Branch(Branch *_parent, vec3 _pos, vec3 _grow, int _born);

};
