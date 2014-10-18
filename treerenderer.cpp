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
        glDeleteBuffers(1, &leaf_rotations_vbo);
        glDeleteBuffers(1, &leaf_scales_vbo);
    }



/*    std::vector<vec3> branch_verts;
    std::vector<float> branch_radii;
    std::vector<unsigned int> branch_indexes;

    std::vector<vec3> leaf_locations;
    std::vector<mat3> leaf_rotations;
    std::vector<float> leaf_scales;

    branch_verts.push_back(vec3(0,0,0));
    branch_radii.push_back(0.f);
    int index = 1;
    std::vector<Branch*> branches = tree->get_branches();
    for (Branch* b : branches){
        branch_verts.push_back(b->position);
        branch_radii.push_back(sqrt(b->radius));
        b->index = index++;
    }
    for (Branch* b : branches){
        if (b->parent && b->parent->parent){

            branch_indexes.push_back(b->parent->parent->index);
            branch_indexes.push_back(b->parent->index);
            branch_indexes.push_back(b->index);
            branch_indexes.push_back(0); // next adjacent is unused

            if (b->radius < tree->leaf_twig_max_size){
                leaf_locations.push_back(b->position);
                leaf_rotations.push_back(b->rotation);
                leaf_scales.push_back(sqrt(b->radius));
            }
        }
    }
*/



    // generate verticies
    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.branch_verts.size() * sizeof (vec3), &tree->vertex_lists.branch_verts[0], GL_STATIC_DRAW);

    // generate branch radii
    glGenBuffers (1, &size_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.branch_radii.size() * sizeof (float), &tree->vertex_lists.branch_radii[0], GL_STATIC_DRAW);

    // generate branch element buffer
    glGenBuffers (1, &(branch_elements.element_buffer));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, tree->vertex_lists.branch_indexes.size() * sizeof (unsigned int), &tree->vertex_lists.branch_indexes[0], GL_STATIC_DRAW);
    branch_elements.size = tree->vertex_lists.branch_indexes.size();   

    // generate leaf element buffer
    /*glGenBuffers (1, &(leaf_elements.element_buffer));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, leaf_elements.element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, leaf_location_indexes.size() * sizeof (unsigned int), &leaf_location_indexes[0], GL_STATIC_DRAW);
    leaf_elements.size = leaf_location_indexes.size();*/

/*    leaf_locations.clear();
    leaf_locations.push_back(vec3(0,95,0));
    leaf_axes.push_back(vec3(0.,0.,0.));
    leaf_rotation.push_back(0.);*/


/*    glGenBuffers(1, &leaf_axis_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_axis_vbo);
    glBufferData(GL_ARRAY_BUFFER, leaf_axes.size() * sizeof(vec3), &leaf_axes[0], GL_STATIC_DRAW);

    glGenBuffers(1, &leaf_rotation_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_rotation_vbo);
    glBufferData(GL_ARRAY_BUFFER, leaf_rotation.size() * sizeof(float), &leaf_rotation[0], GL_STATIC_DRAW);*/

    glGenBuffers(1, &leaf_rotations_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_rotations_vbo);
    glBufferData(GL_ARRAY_BUFFER, tree->vertex_lists.leaf_rotations.size() * sizeof(mat3), &tree->vertex_lists.leaf_rotations[0], GL_STATIC_DRAW);

    glGenBuffers(1, &leaf_scales_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, leaf_scales_vbo);
    glBufferData(GL_ARRAY_BUFFER, tree->vertex_lists.leaf_scales.size() * sizeof(float), &tree->vertex_lists.leaf_scales[0], GL_STATIC_DRAW);

    glGenBuffers (1, &(leaf_elements.element_buffer));
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.leaf_locations.size() * sizeof (vec3), &tree->vertex_lists.leaf_locations[0], GL_STATIC_DRAW);
    leaf_elements.size = tree->vertex_lists.leaf_locations.size();

}

void TreeRenderer::render(glm::mat4 projection, glm::mat4 view){
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0, -50, 0));


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
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glDrawElements (GL_LINES_ADJACENCY, branch_elements.size, GL_UNSIGNED_INT, NULL);
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);




    //leaf_shader.load();
    leaf_shader.activate();
    leaf_shader.set_model(model);
    leaf_shader.set_view(view);
    leaf_shader.set_projection(projection);

    leaf_model.bind();

    glEnableVertexAttribArray(3);
    glBindBuffer (GL_ARRAY_BUFFER, leaf_elements.element_buffer);
    glVertexAttribPointer (3, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), (void*)0);
    glVertexAttribDivisor(3, 1);

    int rotation_attribute_location = 4;
    for (int i=0;i<3;i++){
        glEnableVertexAttribArray(rotation_attribute_location+i);
    }
    glBindBuffer (GL_ARRAY_BUFFER, leaf_rotations_vbo);
    for (int i=0;i<3;i++){
        glVertexAttribPointer (rotation_attribute_location+i, 3, GL_FLOAT, GL_FALSE, sizeof(mat3), (void*)(i*sizeof(vec3)));
        glVertexAttribDivisor(rotation_attribute_location+i, 1);
    }

    glEnableVertexAttribArray(7);
    glBindBuffer (GL_ARRAY_BUFFER, leaf_scales_vbo);
    glVertexAttribPointer (7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);
    glVertexAttribDivisor(7, 1);

    glDrawArraysInstanced(GL_TRIANGLES, 0, leaf_model.size, leaf_elements.size);


    if (draw_attraction_points){
        GLuint tvertex_vbo, tsize_vbo, vao;
        std::vector<vec3> verts;
        std::vector<float> sizes;
    	for (AttractionPoint* attraction_point : tree->get_attraction_points()){
        	verts.push_back(attraction_point->position);
        	sizes.push_back(2);
        }
        point_shader.activate();
        point_shader.set_model(model);
        point_shader.set_view(view);
        point_shader.set_projection(projection);

        glGenBuffers (1, &tvertex_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, tvertex_vbo);
        glBufferData (GL_ARRAY_BUFFER, verts.size() * sizeof (vec3), &verts[0], GL_STATIC_DRAW);

        glGenBuffers (1, &tsize_vbo);
        glBindBuffer (GL_ARRAY_BUFFER, tsize_vbo);
        glBufferData (GL_ARRAY_BUFFER, sizes.size() * sizeof (float), &sizes[0], GL_STATIC_DRAW);

        glBindBuffer (GL_ARRAY_BUFFER, tvertex_vbo);
        glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

        glBindBuffer (GL_ARRAY_BUFFER, tsize_vbo);
        glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL);

        glEnableVertexAttribArray (0);
        glEnableVertexAttribArray (1);
        glDrawArrays (GL_POINTS, 0, verts.size());

        glDeleteBuffers(1, &tvertex_vbo);
        glDeleteBuffers(1, &tsize_vbo);
    }

}


