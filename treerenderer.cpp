#include "treerenderer.hpp"

TreeRenderer::TreeRenderer(Tree* _tree): tree(_tree){
	branch_shader.load();
	leaf_shader.load();
    point_shader.load();
}

void TreeRenderer::regenerate(){
    if (branch_elements.size == 0){
        glDeleteBuffers(1, &(branch_elements.element_buffer));
        glDeleteBuffers(1, &leaf_elements.element_buffer);
        glDeleteBuffers(1, &vertex_vbo);
        glDeleteBuffers(1, &size_vbo);
    }



    std::vector<vec3> verts;
    std::vector<vec3> leaf_locations;
    std::vector<float> branch_radius;
    //std::vector<float> branch_radius;
    std::vector<unsigned int> indexs;
    std::vector<unsigned int> leaf_location_indexes;

    verts.push_back(vec3(0,0,0));
    branch_radius.push_back(0.f);
    int index = 1;
    std::vector<Branch*> branches = tree->get_branches();
    for (Branch* b : branches){
        verts.push_back(b->position);
        branch_radius.push_back(sqrt(b->radius));
        b->index = index++;
    }
    for (Branch* b : branches){
        if (b->parent && b->parent->parent){

            indexs.push_back(b->parent->parent->index);
            indexs.push_back(b->parent->index);
            indexs.push_back(b->index);
            indexs.push_back(0); // next adjacent is unused

            if (b->radius < tree->leaf_twig_max_size){
                leaf_locations.push_back(b->position);
                leaf_location_indexes.push_back(b->index);
            }
        }
    }


    // generate verticies
    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, verts.size() * sizeof (vec3), &verts[0], GL_STATIC_DRAW);

    // generate branch radii
    glGenBuffers (1, &size_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glBufferData (GL_ARRAY_BUFFER, branch_radius.size() * sizeof (float), &branch_radius[0], GL_STATIC_DRAW);

    // generate branch element buffer
    glGenBuffers (1, &(branch_elements.element_buffer));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, indexs.size() * sizeof (unsigned int), &indexs[0], GL_STATIC_DRAW);
    branch_elements.size = indexs.size();   

    // generate leaf element buffer
    /*glGenBuffers (1, &(leaf_elements.element_buffer));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, leaf_elements.element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, leaf_location_indexes.size() * sizeof (unsigned int), &leaf_location_indexes[0], GL_STATIC_DRAW);
    leaf_elements.size = leaf_location_indexes.size();*/

    glGenBuffers (1, &(leaf_elements.element_buffer));
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glBufferData (GL_ARRAY_BUFFER, leaf_locations.size() * sizeof (vec3), &leaf_locations[0], GL_STATIC_DRAW);
    leaf_elements.size = leaf_locations.size();

}

void TreeRenderer::render(glm::mat4 projection, glm::mat4 view){
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0, -100, 0));


    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);

    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);



    //branch_shader.load(); // debuging the shader - live reload
    branch_shader.activate();
    branch_shader.set_model(model);
    branch_shader.set_view(view);
    branch_shader.set_projection(projection);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glDrawElements (GL_LINES_ADJACENCY, branch_elements.size, GL_UNSIGNED_INT, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


    //leaf_shader.load();
    leaf_shader.activate();
    leaf_shader.set_model(model);
    leaf_shader.set_view(view);
    leaf_shader.set_projection(projection);
    leaf_model.bind();

    glEnableVertexAttribArray(2);
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, 1);
    glDrawArraysInstanced(GL_TRIANGLES, 0, leaf_model.size, leaf_elements.size);


     return;}/*

    point_shader.activate();
    point_shader.set_model(model);
    point_shader.set_view(view);
    point_shader.set_projection(projection);
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);
    glDrawArrays (GL_POINTS, 0, leaf_elements.size);


/*    glEnableVertexAttribArray (2);
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glVertexAttribDivisor(2, leaf_model.size);*/

//    glDrawArrays (GL_TRIANGLES, 0, 3);
//    glDrawArrays (GL_TRIANGLES, 0, leaf_model.size);
    //glBindVertexArray(0);
    /*

   

   	verts.clear();
   	sizes.clear();
	for (AttractionPoint* attraction_point : tree->get_attraction_points()){
    	verts.push_back(attraction_point->position);
    	sizes.push_back(2.f);
    }
    point_shader.activate();

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
    glDrawArrays (GL_POINTS, 0, verts.size());

    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &size_vbo);
    glDeleteBuffers(1, &vao);

}

*/
