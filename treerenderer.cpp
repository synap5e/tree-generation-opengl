#include "treerenderer.hpp"

TreeRenderer::TreeRenderer(Tree* _tree): tree(_tree){
	branch_shader.load();
	leaf_shader.load();
    point_shader.load();
    grid_shader.load();
    branch_elements.size = 0;
}

void TreeRenderer::regenerate(){
    if (branch_elements.size == 0){
        glDeleteBuffers(1, &(branch_elements.element_buffer));
        glDeleteBuffers(1, &leaf_elements.element_buffer);
        glDeleteBuffers(1, &vertex_vbo);
        glDeleteBuffers(1, &depth_vbo);
        glDeleteBuffers(1, &size_vbo);
        glDeleteBuffers(1, &leaf_rotations_vbo);
        glDeleteBuffers(1, &leaf_scales_vbo);
    }


    // generate verticies
    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.branch_verts.size() * sizeof (vec3), &tree->vertex_lists.branch_verts[0], GL_STATIC_DRAW);

    glGenBuffers (1, &depth_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, depth_vbo);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.depths.size() * sizeof (float), &tree->vertex_lists.depths[0], GL_STATIC_DRAW);

    // generate branch radii
    glGenBuffers (1, &size_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glBufferData (GL_ARRAY_BUFFER, tree->vertex_lists.branch_radii.size() * sizeof (float), &tree->vertex_lists.branch_radii[0], GL_STATIC_DRAW);

    // generate branch element buffer
    glGenBuffers (1, &(branch_elements.element_buffer));
    glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glBufferData (GL_ELEMENT_ARRAY_BUFFER, tree->vertex_lists.branch_indexes.size() * sizeof (unsigned int), &tree->vertex_lists.branch_indexes[0], GL_STATIC_DRAW);
    branch_elements.size = tree->vertex_lists.branch_indexes.size();   

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

void TreeRenderer::render(glm::mat4 projection, glm::mat4 view, glm::mat4 model, glm::vec3 light){
    
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glEnableVertexAttribArray (0);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, size_vbo);
    glEnableVertexAttribArray (1);
    glVertexAttribPointer (1, 1, GL_FLOAT, GL_FALSE, 0, NULL); 
    
    glBindBuffer (GL_ARRAY_BUFFER, depth_vbo);
    glEnableVertexAttribArray (2);
    glVertexAttribPointer (2, 1, GL_FLOAT, GL_FALSE, 0, NULL);


    //branch_shader.load(); // debuging the shader - live reload
    branch_shader.activate2();
    branch_shader.set_model(model);
    branch_shader.set_view(view);
    branch_shader.set_projection(projection);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, branch_elements.element_buffer);
    glDrawElements (GL_LINES_ADJACENCY, branch_elements.size, GL_UNSIGNED_INT, NULL);
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);



    //leaf_shader.load();
    leaf_shader.activate();
    leaf_shader.set_model(model);
    leaf_shader.set_view(view);
    leaf_shader.set_projection(projection);

    // enables and uses attrib 0-2
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
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(4);
    glDisableVertexAttribArray(5);
    glDisableVertexAttribArray(6);
    glDisableVertexAttribArray(7);

    if (draw_attraction_points){
        GLuint tvertex_vbo, tsize_vbo, vao;
        std::vector<vec3> verts;
        std::vector<float> sizes;
    	for (AttractionPoint* attraction_point : tree->get_attraction_points()){
        	verts.push_back(attraction_point->position);
        	sizes.push_back(attraction_point->weight + 0.1);
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


