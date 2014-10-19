#include "tree.hpp"

#include "random.hpp"
#include "glm/gtx/string_cast.hpp"

Tree::Tree(vec3 pos, picojson::object tree_params, int _seed_timer){
    position = pos;
    seed_timer = _seed_timer;

    params.radius                  = tree_params["radius"].get<double>();
    params.height                  = tree_params["height"].get<double>();
    params.root_height             = tree_params["root_height"].get<double>();
    params.canopy_exponent         = tree_params["canopy_exponent"].get<double>();
    params.initial_attraction_points = tree_params["initial_attraction_points"].get<double>();
    params.attraction_point_creation_rate = tree_params["attraction_point_creation_rate"].get<double>();
    params.branch_length           = tree_params["branch_length"].get<double>();
    params.kill_distance           = tree_params["kill_distance"].get<double>();
    params.influence_distance      = tree_params["influence_distance"].get<double>();
    params.twig_max_descendants    = tree_params["twig_max_descendants"].get<double>();
    params.soft_bends_weight       = tree_params["soft_bends_weight"].get<double>();
    params.branch_kill_age         = tree_params["branch_kill_age"] .get<double>();

    init_branch_parser(branch_radius_parser, tree_params["branch_radius"].get<picojson::object>());
    init_branch_parser(leaf_check_parser, tree_params["leaf_check"].get<picojson::object>());
    init_branch_parser(leaf_size_parser, tree_params["leaf_size"].get<picojson::object>());

    std::string err = picojson::get_last_error();
    if (! err.empty()) {
        std::cerr << err << std::endl;
        exit(-1);
    }

    generate_crown(params.initial_attraction_points);
    generate_trunk();
}

void Tree::init_branch_parser(mu::Parser &parser, picojson::object function_params){
    picojson::object constants = function_params["constants"].get<picojson::object>();
    for (auto &kv : constants){
        parser.DefineConst(kv.first, kv.second.get<double>());
    }

    parser.DefineVar("descendants", &(brach_radius_variables.descendants)); 
    parser.DefineVar("age", &(brach_radius_variables.age)); 
    parser.DefineVar("depth", &(brach_radius_variables.depth)); 

    parser.SetExpr(function_params["function"].get<std::string>());
}

void Tree::generate_crown(int count){
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

    float mx = 0;
    for (int i=0; i< count; ++i){
        float yu = RandomGen::get(0, 1);
        
        float theta = acosf(1-2*yu);
        float phi = RandomGen::get(0, PI);

        float xrad = 0.5 * (1-cosf(theta)) * sinf(theta) * cosf(phi);
        float zrad = 0.5 * (1-cosf(theta)) * sinf(theta) * sinf(phi);
        xrad = pow(xrad, params.canopy_exponent);
        zrad = pow(zrad, params.canopy_exponent);
       
        vec3 location = vec3(   RandomGen::get(-xrad, xrad) * params.radius,
                                yu * params.height,
                                RandomGen::get(-zrad, zrad) * params.radius);
        if (location.y < params.root_height){
            continue;
        }
        mx = max(location.x, mx);
        attraction_points.push_back(new AttractionPoint(location + position));

    }
   // printf("%f\n", mx);

}

Tree::~Tree(){
    // TODO: delete the things
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
    ++simulation_time;
    if (simulation_time < seed_timer) return true;

    for (int i=0;i<params.attraction_point_creation_rate; ++i){
        attraction_points.push_back(new AttractionPoint(
                                        vec3(
                                        RandomGen::get(-params.radius, params.radius), 
                                        RandomGen::get(0, params.height),
                                        RandomGen::get(-params.radius, params.radius))));
    }
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
            attraction_point->closest->grow_direction += dir * attraction_point->weight;
            attraction_point->closest->grow_count += fabs(attraction_point->weight);
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


    /*if (new_branches.empty()){
        //return;
        for (AttractionPoint* l : attraction_points){
            delete l;
        }
        std::cout << "Finished with " << attraction_points.size() << " unused points\n";
    	attraction_points.clear();
        return false;
    }*/

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

        // fill out the variables used by the parser
        brach_radius_variables.descendants = b->descendants;
        brach_radius_variables.age = ageof(b);
        brach_radius_variables.depth = b->depth;

        vertex_lists.branch_radii.push_back(branch_radius_parser.Eval());
        b->index = index++;

        if (leaf_check_parser.Eval()){
            vertex_lists.leaf_locations.push_back(b->position);
            vertex_lists.leaf_rotations.push_back(b->rotation);

            vertex_lists.leaf_scales.push_back(leaf_size_parser.Eval());
        }
    }
    for (Branch* b : branches){
        if (b->parent && b->parent->parent){

            vertex_lists.branch_indexes.push_back(b->parent->parent->index);
            vertex_lists.branch_indexes.push_back(b->parent->index);
            vertex_lists.branch_indexes.push_back(b->index);
            vertex_lists.branch_indexes.push_back(0); // next adjacent is unused
        }
    }
}

void Tree::update(VoxelGrid *grid, vec3 light){
    for (Branch *b : branches){
        grid->add(b->position);
    }
    for (int i=0; i <attraction_points.size(); i++){
        AttractionPoint *a = attraction_points[i];
        int intersections = grid->cast(a->position, normalize(light - a->position));
        a->weight = fmax(-20, 100 - intersections)/100.f;

        if (a->weight == 0){
            attraction_points.erase(attraction_points.begin()+i);
            --i;
        }
    }
   // printf("Shadow level at (0,0,0) is %d\n", grid->cast(vec3(0,0,0), vec3(0,1,0)));
}

std::vector<AttractionPoint*> Tree::get_attraction_points(){
    return attraction_points; 
};
