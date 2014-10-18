#include "tree.hpp"

#include "random.hpp"
#include "glm/gtx/string_cast.hpp"

Tree::Tree(){
	generate_crown();
	generate_trunk();
}


Tree::Tree(picojson::object tree_params){
    params.radius                  = tree_params["radius"].get<double>();
    params.height                  = tree_params["height"].get<double>();
    params.root_height             = tree_params["root_height"].get<double>();
    params.canopy_exponent         = tree_params["canopy_exponent"].get<double>();
    params.attraction_point_count  = tree_params["attraction_point_count"].get<double>();
    params.branch_length           = tree_params["branch_length"].get<double>();
    params.kill_distance           = tree_params["kill_distance"].get<double>();
    params.influence_distance      = tree_params["influence_distance"].get<double>();
    params.initial_radius          = tree_params["initial_radius"].get<double>();
    params.radius_growth           = tree_params["radius_growth"].get<double>();
    params.twig_max_descendants    = tree_params["twig_max_descendants"].get<double>();
    params.soft_bends_weight       = tree_params["soft_bends_weight"].get<double>();
    params.branch_kill_age         = tree_params["branch_kill_age"] .get<double>();

    std::string err = picojson::get_last_error();
    if (! err.empty()) {
        std::cerr << err << std::endl;
        exit(-1);
    }

    generate_crown();
    generate_trunk();
}

void Tree::generate_crown(){
	position = vec3(0,0,0);

    float PI = 3.141592653589793f;

    /*

    The unit teardrop is defined by 
    x = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi)
    y = 0.5 * (1-cosf(theta))
    z = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi)

    0 <= theta <= 2pi
    0 <= phi <= pi

    so

    choose a y from [0,1]
    theta = acos(1-2y)
    choose a phi [0,pi]
    
    */

    for (int i=0; i< params.attraction_point_count; ++i){
        float yu = RandomGen::get(0, 1);
        
        float theta = acosf(1-2*yu);
        float phi = RandomGen::get(0, PI);

        float rad = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi);
        rad = pow(rad, params.canopy_exponent);
       
        vec3 location = vec3(   RandomGen::get(-rad, rad) * params.radius,
                                yu * params.height,
                                RandomGen::get(-rad, rad) * params.radius);
        if (location.y < params.root_height){
            continue;
        }
        attraction_points.push_back(new AttractionPoint(location));

    }

}

void Tree::generate_trunk(){
	root = new Branch(nullptr, position + vec3(0, params.root_height, 0), vec3(0.f, 1.f, 0.f), 0);
//    root->radius = 15;
	branches.push_back(root);
    live_branches.push_back(root);

}

int Tree::ageof(Branch* b){
    return simulation_time - b->born;
}

int r = 0;
bool Tree::grow(){
    for (int i = 0; i < attraction_points.size(); ++i) {
    	AttractionPoint* attraction_point = attraction_points[i];

        attraction_point->closest = nullptr;
        vec3 direction;

        bool attraction_point_killed = false;

		for (Branch* b  : live_branches){
            direction = attraction_point->position - b->position;
            float distance = length(direction);
            direction = normalize(direction);


            if (distance < params.kill_distance * params.branch_length){
            	//printf("eat attraction_point\n");
                attraction_points.erase(attraction_points.begin()+i);                        
                i--;
                attraction_point_killed = true;
                break;
            } else if (distance < params.influence_distance * params.branch_length
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
    for (int i=0;i<live_branches.size();i++){
        Branch* b = live_branches[i];
        if (b->grow_count > 0){
        	//std::cout << "Add branch, grow count:" << b->grow_count << "\n";

            vec3 avgDirection = b->grow_direction / float(b->grow_count+params.soft_bends_weight);
            avgDirection = normalize(avgDirection);

            b->grow_direction = b->original_grow_direction*float(params.soft_bends_weight);
            b->grow_count = 0;

            /*if (RandomGen::get(0, 2) > 1){
                continue;
            }*/


			vec3 new_pos = b->position + avgDirection * params.branch_length;

			if (std::find(branch_locations.begin(), branch_locations.end(), new_pos)!=branch_locations.end()){
				continue;
			}
			branch_locations.push_back(new_pos);

            Branch* newBranch = new Branch(b, new_pos, avgDirection, simulation_time);
            new_branches.push_back(newBranch);
            
            int pc = 0;
            Branch *parent = b->parent;
            while (parent){
                //std::cout << parent->descendants << " -> ";
            	++(parent->descendants);
                //std::cout << parent->descendants << "\n";
            	parent = parent->parent;
            }
        } else {
            if (ageof(b) > params.branch_kill_age){
                live_branches.erase(live_branches.begin()+i);
                ++i;
            }
        }

    }

    for (Branch* nb : new_branches){
        branches.push_back(nb);
        live_branches.push_back(nb);
    }


    if (new_branches.empty()){
        //return;
        for (AttractionPoint* l : attraction_points){
            delete l;
        }
        std::cout << "Finished with " << attraction_points.size() << " unused points\n";
    	attraction_points.clear();
        return false;
       /* for (Branch* b : branches){
            delete b;
        }
        branches.clear();
        generate_crown();
        generate_trunk();*/
    }

    ++simulation_time;
    return true;
}


void Tree::regenerate_vertex_lists(){
    vertex_lists.branch_verts.clear();
    vertex_lists.branch_radii.clear();
    vertex_lists.branch_indexes.clear();
    vertex_lists.leaf_locations.clear();
    vertex_lists.leaf_rotations.clear();
    vertex_lists.leaf_scales.clear();

    vertex_lists.branch_verts.push_back(vec3(0,0,0));
    vertex_lists.branch_radii.push_back(0.f);
    int index = 1;
    for (Branch* b : branches){
        vertex_lists.branch_verts.push_back(b->position);

        float radius = sqrt(params.radius_growth * b->descendants + params.initial_radius);
        vertex_lists.branch_radii.push_back(radius);
        b->index = index++;
    }
    for (Branch* b : branches){
        if (b->parent && b->parent->parent){

            vertex_lists.branch_indexes.push_back(b->parent->parent->index);
            vertex_lists.branch_indexes.push_back(b->parent->index);
            vertex_lists.branch_indexes.push_back(b->index);
            vertex_lists.branch_indexes.push_back(0); // next adjacent is unused

            if (b->descendants <= params.twig_max_descendants){
                vertex_lists.leaf_locations.push_back(b->position);
                vertex_lists.leaf_rotations.push_back(b->rotation);

                float leaf_size = sqrt(b->descendants + 0.01);
                vertex_lists.leaf_scales.push_back(leaf_size);
            }
        }
    }
}

std::vector<AttractionPoint*> Tree::get_attraction_points(){
    return attraction_points; 
};
