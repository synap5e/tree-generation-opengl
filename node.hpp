#pragma once  

class Node;

#include "stem.hpp"
#include "render.hpp"


#include <vector>
#include <map>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// see http://en.wikipedia.org/wiki/Plant_stem for terminology
// a node holds a bud which grows into some form of stem
class Node{
protected:

public:
	Stem* parent;
	std::vector<Stem*> buds;


	Node(Stem* _parent);
	void simulate(float delta);
	void enqueue(std::map<std::string, std::vector<RenderComponent>>& objects, glm::mat4& model);
};
