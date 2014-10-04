#include "stem.hpp"

#include <math.h>

void Stem::feed(float energy){
	stored_energy += energy;
}

Branch::Branch(Node* _parent) : Stem(_parent){
	//Branch* attached_to = static_cast<Branch*>(_parent->parent);
	Stem* attached_to = _parent->parent;

	float allowed_rotation;
	if (attached_to == nullptr){
		// trunk - grow straight up
		allowed_rotation = 0;
	} else /*if (attached_to->length < )*/{
		allowed_rotation = 1;
	}

	length = 0;
	rotation = 	glm::angleAxis(	RandomGen::get(0, 360), 	 							glm::vec3(0,1,0)) * 
				glm::angleAxis(	RandomGen::get(-allowed_rotation, allowed_rotation), 	glm::vec3(1,0,0)) * 
				glm::angleAxis(	RandomGen::get(-allowed_rotation, allowed_rotation), 	glm::vec3(0,0,1));
}

void Branch::simulate(float delta){
	length+= delta;
	for (auto &kv : nodes){
		Node* n = kv.second;
		n->simulate(delta);
	}

	if (nodes.size() < 2 && length > 1 && RandomGen::get(0, 1000) < length){
		nodes[RandomGen::get(0, length)] = new Node(this);
		//printf("new node\n");
	}
}

void Branch::generate_mesh(){
	if (mesh){
		delete mesh;
	}
	float c2px = powf(25, length/100.f);
	radius=(c2px-1)/(c2px)+0.1;
	mesh = new Mesh(length, radius);
	for (auto &kv : nodes){
		Node* n = kv.second;
		n->generate_mesh();
	}
}

void Branch::enqueue(std::map<std::string, std::vector<RenderComponent>>& objects, glm::mat4& model){

    model = model * glm::mat4_cast(rotation);

    RenderComponent ob;
    ob.vertex[0] = glm::vec3(model * glm::vec4(0,0,0,1));
    ob.vertex[1] = glm::vec3(model * glm::vec4(0,length,0, 1));

    objects["branch"].push_back(ob);

	for (auto &kv : nodes){
		//printf("recur to next %f\n", kv.first);
		Node* n = kv.second;

		//std::cout << kv.first << "\n";
		glm::mat4 newmodel = glm::translate(model, glm::vec3(0.f, kv.first, 0.f));
		n->enqueue(objects, newmodel);
	}
}

