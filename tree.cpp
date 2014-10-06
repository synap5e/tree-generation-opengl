#include "tree.hpp"

#include "random.hpp"
#include "glm/gtx/string_cast.hpp"

Tree::Tree(){
	generate_crown();
	generate_trunk();
}


void Tree::generate_crown(){
	position = vec3(0,0,0);

    float PI = 3.141592653589793f;

    /*

    Teardrop is defined by 
    x = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi)
    y = 0.5 * (1-cosf(theta))
    z = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi)

    0 <= theta <= 2pi
    0 <= phi <= pi

    so

    theta = acos(1-2y)
    with 0 <= phi <= pi


    */

    for (int i=0; i<attraction_point_count; ++i){
        float yu = RandomGen::get(0, 1);
        
        float theta = acosf(1-2*yu);
        float phi = RandomGen::get(0, PI);

        float xrad = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi);
        float zrad = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi);
       
        vec3 location = vec3(   RandomGen::get(-xrad, xrad) * radius,
                                yu * height,
                                RandomGen::get(-zrad, zrad) * radius);
        attraction_points.push_back(new AttractionPoint(location));

/*        vec3 location = vec3(   xrad,
                                yu*2,
                                zrad) * 100.f;
        attraction_points.push_back(new AttractionPoint(location));

        location = vec3(-xrad,
                        yu*2,
                        -zrad) * 100.f;
        attraction_points.push_back(new AttractionPoint(location));*/

    }

}

void Tree::generate_trunk(){
	root = new Branch(nullptr, position, vec3(0.f, 1.f, 0.f));
//    root->radius = 15;
	branches.push_back(root);

/*	Branch* current = new Branch(root, vec3(position.x, position.y + branch_length*10, position.z), vec3(0.f, 1.f, 0.f));
    current->radius = 15;
	branches.push_back(current);

    current = new Branch(current, vec3(position.x-20, position.y + branch_length*50, position.z), vec3(0.f, 1.f, 0.f));
    current->radius = 15;
    branches.push_back(current);

    current = new Branch(current, vec3(position.x + 20, position.y + branch_length*100, position.z), vec3(0.f, 1.f, 0.f));
    current->radius = 15;
    branches.push_back(current);*/

    /*

	while (length(root->position - current->position) < trunk_height) {
	    Branch* trunk = new Branch(current, vec3(current->position.x, current->position.y + branch_length, current->position.z), vec3(0.f, 1.f, 0.f));
        trunk->radius = initial_radius;// * (trunk_height / branch_length);
	    current = trunk; 
	    //std::cout << "insert at " << to_string(current->position) << "\n";
	    branches.push_back(current);
	}   */

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


            if (distance < kill_distance * branch_length){
            	//printf("eat attraction_point\n");
                attraction_points.erase(attraction_points.begin()+i);                        
                i--;
                attraction_point_killed = true;
                break;
            } else if (distance < influence_distance * branch_length
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

            vec3 avgDirection = b->grow_direction / float(b->grow_count+soft_bends_weight);
            avgDirection = normalize(avgDirection);

            b->grow_direction = b->original_grow_direction*float(soft_bends_weight);
            b->grow_count = 0;

            /*if (RandomGen::get(0, 2) > 1){
                continue;
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
        //return;
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
    std::vector<unsigned int> indexs;

    verts.push_back(vec3(0,0,0));
    sizes.push_back(0.f);
    int index = 1;
    for (Branch* b  : branches){
        verts.push_back(b->position);
        sizes.push_back(sqrt(b->radius));
        b->index = index++;
    }
    for (Branch* b  : branches){
        if (b->parent && b->parent->parent){

            indexs.push_back(b->parent->parent->index);
            indexs.push_back(b->parent->index);
            indexs.push_back(b->index);
            indexs.push_back(0); // next adjacent is unused
        }
    }


    bs.activate();

    GLuint element_buffer;
    GLuint vertex_vbo; 
    GLuint size_vbo;

    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, verts.size() * sizeof (vec3), &verts[0], GL_STATIC_DRAW);

    glGenBuffers (1, &size_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glBufferData (GL_ARRAY_BUFFER, sizes.size() * sizeof (float), &sizes[0], GL_STATIC_DRAW);

    glGenBuffers (1, &element_buffer);
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexs.size() * sizeof (unsigned int), &indexs[0], GL_STATIC_DRAW);

    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_buffer);
    glDrawElements (GL_LINES_ADJACENCY, indexs.size(), GL_UNSIGNED_INT, (void*)0);

    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &size_vbo);
    glDeleteBuffers(1, &element_buffer);

//   	render(verts, sizes, GL_LINES_ADJACENCY);

   	verts.clear();
   	sizes.clear();
	for (AttractionPoint* attraction_point : attraction_points){
    	verts.push_back(attraction_point->position);
    	sizes.push_back(2.f);
    }
    point_shader.activate();
    render(verts, sizes, GL_POINTS);



}
