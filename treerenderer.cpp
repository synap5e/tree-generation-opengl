#include "treerenderer.hpp"

TreeRenderer::TreeRenderer(Tree* _tree): tree(_tree){
	branch_shader.load();
	point_shader.load();

}


void TreeRenderer::render(glm::mat4 projection, glm::mat4 view){
    glm::mat4 model = glm::mat4(1);
    model = glm::translate(model, glm::vec3(0, -100, 0));

    //branch_shader.load(); // debuging the shader - live reload
    branch_shader.activate();
    branch_shader.set_model(model);
    branch_shader.set_view(view);
    branch_shader.set_projection(projection);

    point_shader.activate();
    point_shader.set_model(model);
    point_shader.set_view(view);
    point_shader.set_projection(projection);





    std::vector<vec3> verts;
    std::vector<float> sizes;
    std::vector<unsigned int> indexs;
    std::vector<unsigned int> leaf_location_index;

    verts.push_back(vec3(0,0,0));
    sizes.push_back(0.f);
    int index = 1;
    std::vector<Branch*> branches = tree->get_branches();
    for (Branch* b : branches){
        verts.push_back(b->position);
        sizes.push_back(sqrt(b->radius));
        b->index = index++;
    }
    for (Branch* b : branches){
        if (b->parent && b->parent->parent){

            indexs.push_back(b->parent->parent->index);
            indexs.push_back(b->parent->index);
            indexs.push_back(b->index);
            indexs.push_back(0); // next adjacent is unused
        }

        if (b->radius < tree->leaf_twig_max_size){
            leaf_location_index.push_back(b->index);
        }
    }

    branch_shader.activate();

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

    
    glDeleteBuffers(1, &size_vbo);
    glDeleteBuffers(1, &element_buffer);


    //TODO: USE glDrawArraysInstanced/glDrawElementsInstanced to draw leaves
    // leaf_location_index indexes the verticies in vertex_vbo at positions to draw leaves


    glDeleteBuffers(1, &vertex_vbo);


    return;

   	verts.clear();
   	sizes.clear();
	for (AttractionPoint* attraction_point : tree->get_attraction_points()){
    	verts.push_back(attraction_point->position);
    	sizes.push_back(2.f);
    }
    point_shader.activate();
    
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
    glDrawArrays (GL_POINTS, 0, verts.size());

    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &size_vbo);
    glDeleteBuffers(1, &vao);

/*

}


   tree->draw(branch_shader, point_shader);*/
}

