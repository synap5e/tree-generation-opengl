#pragma once  

class Stem;

#include "node.hpp"
#include "mesh.hpp"
#include "random.hpp"
#include "shader.hpp"
#include "render.hpp"





#include <map>
#include <vector>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"

// see http://en.wikipedia.org/wiki/Plant_stem for terminology
class Stem { // AKA internode (or edge if you like the _other_ sort of tree)
public:
	Node* parent;
	float stored_energy;

	Mesh *mesh = nullptr;

	// rotation to new y along stem
	glm::quat rotation;

	

	Stem(Node* _parent): parent(_parent){};
	virtual void simulate(float delta) = 0;
	virtual void generate_mesh() = 0;
	virtual void enqueue(std::map<std::string, std::vector<RenderComponent>>& objects, glm::mat4& model) = 0;
	void feed(float energy);
};

class Branch : public Stem{ // branched steam
protected:
	float length;
	float radius;

	// nodes along stem
	std::map<float, Node*> nodes;
public:
	Branch(Node* _parent);
	void simulate(float delta);
	void enqueue(std::map<std::string, std::vector<RenderComponent>>& objects, glm::mat4& model);

};
