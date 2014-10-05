#include "tree.hpp"

#include "random.hpp"
#include "glm/gtx/string_cast.hpp"

Tree::Tree(){
	generate_crown();
	generate_trunk();
}


void Tree::generate_crown(){
	position = vec3(0,0,0);
	for (int i = 0; i < attraction_point_count; i++) {
	    vec3 location = vec3(	RandomGen::get(position.x - diameter, position.x + diameter),
                            	RandomGen::get(position.y + trunk_height, position.y + trunk_height + height),
                            	RandomGen::get(position.z - diameter, position.z + diameter));

	    AttractionPoint* attraction_point = new AttractionPoint(location);
	    attraction_points.push_back(attraction_point);
	}

}

void Tree::generate_trunk(){
	root = new Branch(nullptr, position, vec3(0.f, 1.f, 0.f));
    root->radius = initial_radius;
	branches.push_back(root);

	Branch* current = new Branch(root, vec3(position.x, position.y + branch_length, position.z), vec3(0.f, 1.f, 0.f));
	branches.push_back(current);

	while (length(root->position - current->position) < trunk_height) {
	    Branch* trunk = new Branch(current, vec3(current->position.x, current->position.y + branch_length, current->position.z), vec3(0.f, 1.f, 0.f));
        trunk->radius = initial_radius;
	    current = trunk; 
	    //std::cout << "insert at " << to_string(current->position) << "\n";
	    branches.push_back(current);
	}    

	/*printf("{\n");
	for (Branch* b : branches){
		std::cout << "\t" << to_string(b->position) << " : " << (void*)*(&b) << "\n";
		//Branch* b = kv.second;

	}
	std::cout << "}\n";*/
}

int r = 0;
void Tree::grow(){
	//return;
    if (attraction_points.empty()) { 
        return; 
    }
    for (int i = 0; i < attraction_points.size(); ++i) {
    	AttractionPoint* attraction_point = attraction_points[i];

        attraction_point->closest = nullptr;
        vec3 direction;

        bool attraction_point_killed = false;

		for (Branch* b  : branches){
            direction = attraction_point->position - b->position;
            float distance = length(direction);
            direction = normalize(direction);

            if (distance < kill_distance){
            	//printf("eat attraction_point\n");
                attraction_points.erase(attraction_points.begin()+i);                        
                i--;
                attraction_point_killed = true;
                break;
            } else if (distance < influence_distance
            		   && ( attraction_point->closest == nullptr
            			 || length(attraction_point->position - attraction_point->closest->position) > distance)){
                    attraction_point->closest = b;
            }
        }
        

        if (!attraction_point_killed && attraction_point->closest) {
            vec3 dir = attraction_point->position - attraction_point->closest->position;
            dir = normalize(dir);
            attraction_point->closest->grow_direction += dir;
            attraction_point->closest->grow_count++;
        }
        
    }

    std::vector<Branch*> new_branches;
    for (Branch* b : branches){
        if (b->grow_count > 0){
        	//std::cout << "Add branch, grow count:" << b->grow_count << "\n";

            vec3 avgDirection = b->grow_direction / float(b->grow_count+1);
            avgDirection = normalize(avgDirection);

            b->grow_direction = b->original_grow_direction;
            b->grow_count = 0;

           /* if (RandomGen::get(0, 10) > 1){
				return;
			}*/

			vec3 new_pos = b->position + avgDirection * branch_length;

			if (std::find(branch_locations.begin(), branch_locations.end(), new_pos)!=branch_locations.end()){
				continue;
			}
			branch_locations.push_back(new_pos);

            Branch* newBranch = new Branch(b, new_pos, avgDirection);
            new_branches.push_back(newBranch);
            newBranch->radius = initial_radius;

            int pc = 0;
            Branch *parent = b;
            while (parent){
            	parent->radius += radius_growth;
            	parent = parent->parent;
            }
        }

    }

    for (Branch* nb : new_branches){
        branches.push_back(nb);
    }


    if (new_branches.empty()){
        for (AttractionPoint* l : attraction_points){
            delete l;
        }
    	attraction_points.clear();
       /* for (Branch* b : branches){
            delete b;
        }
        branches.clear();
        generate_crown();
        generate_trunk();*/
    }
}

void Tree::render(std::vector<vec3> verts, std::vector<float> sizes, GLenum type){
	GLuint vertex_vbo; 
    GLuint size_vbo;
    GLuint vao; 

    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, verts.size() * sizeof (vec3), &verts[0], GL_STATIC_DRAW);

    glGenBuffers (1, &size_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizes.size() * sizeof (float), &sizes[0], GL_STATIC_DRAW);

    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    glDrawArrays (type, 0, verts.size());

    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &size_vbo);
    glDeleteBuffers(1, &vao);
}

void Tree::draw(BranchShader bs, AttractionPointShader point_shader){

    std::vector<vec3> verts;
    std::vector<float> sizes;
    for (Branch* b  : branches){
		if (b->parent){
        	verts.push_back(b->parent->position);
        	sizes.push_back(b->parent->radius);

        	verts.push_back(b->position);
        	sizes.push_back(b->radius);
		}
    }
    bs.activate();
   	render(verts, sizes, GL_LINES);

   	verts.clear();
   	sizes.clear();
	for (AttractionPoint* attraction_point : attraction_points){
    	verts.push_back(attraction_point->position);
    	sizes.push_back(2.f);
    }
    point_shader.activate();
    render(verts, sizes, GL_POINTS);



}
