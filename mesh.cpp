#include "mesh.hpp"

#include <iostream>

#include <GLFW/glfw3.h>


Mesh::~Mesh() {
    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &colors_vbo);
    glDeleteBuffers(1, &normals_vbo);
    glDeleteVertexArrays(1, &vao);
}

Mesh::Mesh(float height, float radius){

/*    float height = 20.0f;
    float radius = 1.f;
*/
    unsigned short num_sides = 20;
    float PI = 3.141592653589793f;
    float theta = 2.f * PI / float(num_sides);
    float cs = glm::cos(theta);
    float sn = glm::sin(theta);

    // Create vertex array data
    size_t num_vertices = (num_sides + 1) * 2;
    std::vector<glm::vec3> verts(num_vertices);
    std::vector<glm::vec3> normals(num_vertices);
    std::vector<glm::vec3> colors(num_vertices);
    std::vector<glm::vec2> texcoords(num_vertices);

    auto v = begin(verts);
    auto n = begin(normals);
    auto t = begin(texcoords);
    auto c = begin(colors);

    float x = radius;
    float z = 0.0f;
    float nrml,tx;
    for(unsigned short i = 0; i <= num_sides; ++i) {
        tx = (float) i / num_sides;

        // Vertex 1 (bottom)
        nrml = 1.f / glm::sqrt(x*x + z*z);
        *v++ = glm::vec3(x, 0.0f, z) * radius;
        *n++ = glm::vec3(x, 0.0f, z) * nrml;
        *t++ = glm::vec2(tx, 0.0f);
        *c++ = glm::vec3(1.0f, 0.0f,  1.0f);

        // Vertex2 (top)
        *v++ = glm::vec3(x, height / radius, z) * radius;
        *n++ = glm::vec3(x, 0.0f, z) * nrml;
        *t++ = glm::vec2(tx, 1.f);
        *c++ = glm::vec3(1.0f, 1.0f,  0.0f);

        // Next edge
        const float x1 = x;
        x = cs*x  - sn*z;
        z = sn*x1 + cs*z;
    }

    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, num_vertices * sizeof (glm::vec3), &verts[0], GL_STATIC_DRAW);
    
    glGenBuffers (1, &colors_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
    glBufferData (GL_ARRAY_BUFFER, num_vertices * sizeof (glm::vec3), &colors[0], GL_STATIC_DRAW);

    glGenBuffers (1, &normals_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
    glBufferData (GL_ARRAY_BUFFER, num_vertices * sizeof (glm::vec3), &normals[0], GL_STATIC_DRAW);
    
    /* create the VAO.
    we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
    the memory should be fetched for vertex shader input variables 0, and 1,
    respectively. we also have to explicitly enable both 'attribute' variables.
    'attribute' is the older name for vertex shader 'in' variables. */
    size = num_vertices;
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, colors_vbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer (GL_ARRAY_BUFFER, normals_vbo);
    glVertexAttribPointer (2, 3, GL_FLOAT, GL_FALSE, 0, NULL);


    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    glEnableVertexAttribArray (2);

}
