#include "node.hpp"

Node::Node(Stem* _parent) : parent(_parent){};

void Node::simulate(float delta){
	for (Stem *s : buds){
		s->simulate(delta);
	}
	if (buds.empty()){
		//printf("Adding new branch\n");
		buds.push_back(new Branch(this));
	}
}

void Node::generate_mesh(){
	for (Stem *s : buds){
		s->generate_mesh();
	}
}

void Node::enqueue(std::map<std::string, std::vector<RenderComponent>>& objects, glm::mat4& model){
	for (Stem *s : buds){
		s->enqueue(objects, model);
	}
}
