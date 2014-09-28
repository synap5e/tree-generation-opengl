#include "mesh.hpp"

#include <iostream>

#include <GLFW/glfw3.h>

Mesh::Mesh()/*: m_pBody(pBody)*/ {

}

void Mesh::init(){

        double PI = 3.14159;
        double Radius = 5;
        float Resolution = 50;
    
    
    // vectors to hold our data
    // vertice positions
    std::vector<glm::vec3> v;
                                // texture map
    std::vector<glm::vec2> t;
                                // normals
    std::vector<glm::vec3> n;

    std::vector<glm::vec3> c;
    
    // iniatiate the variable we are going to use
    float X1,Y1,X2,Y2,Z1,Z2;
    float inc1,inc2,inc3,inc4,inc5,Radius1,Radius2;

    for(int w = 0; w < Resolution; w++) {
        for(int h = (-Resolution/2); h < (Resolution/2); h++){
        
            inc1 = (w/(float)Resolution)*2*PI;
            inc2 = ((w+1)/(float)Resolution)*2*PI;

            inc3 = (h/(float)Resolution)*PI;
            inc4 = ((h+1)/(float)Resolution)*PI;


            X1 = sin(inc1);
            Y1 = cos(inc1);
            X2 = sin(inc2);
            Y2 = cos(inc2);

            // store the upper and lower radius, remember everything is going to be drawn as triangles
            Radius1 = Radius*cos(inc3);
            Radius2 = Radius*cos(inc4);


            Z1 = Radius*sin(inc3); 
            Z2 = Radius*sin(inc4);

            // insert the triangle coordinates
            v.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
            v.push_back(glm::vec3(Radius1*X2,Z1,Radius1*Y2));
            v.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));



            v.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
            v.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));
            v.push_back(glm::vec3(Radius2*X1,Z2,Radius2*Y1));


            // insert the texture map, im using this to draw texture out from a texture atlas,
            // so you probably want to write your own algorithm for this!
            t.push_back(glm::vec2(0.6666f, 0.5f)*glm::vec2(1,0.25f)); 
            t.push_back(glm::vec2(0.666f, 1.0f)*glm::vec2(1,0.25f)); 
            t.push_back(glm::vec2(1.0f, 1.0f)*glm::vec2(1,0.25f));


            t.push_back(glm::vec2(0.6666f, 0.5f)*glm::vec2(1,0.25f)); 
            t.push_back(glm::vec2(1.00f, 1.0f)*glm::vec2(1,0.25f)); 
            t.push_back(glm::vec2(1.00f, 0.5f)*glm::vec2(1,0.25f));

            // insert the normal data
            n.push_back(glm::vec3(X1,Z1,Y1)/ glm::length(glm::vec3(X1,Z1,Y1)));
            n.push_back(glm::vec3(X2,Z1,Y2)/ glm::length(glm::vec3(X2,Z1,Y2)));
            n.push_back(glm::vec3(X2,Z2,Y2)/ glm::length(glm::vec3(X2,Z2,Y2)));
            n.push_back(glm::vec3(X1,Z1,Y1)/ glm::length(glm::vec3(X1,Z1,Y1)));
            n.push_back(glm::vec3(X2,Z2,Y2)/ glm::length(glm::vec3(X2,Z2,Y2)));
            n.push_back(glm::vec3(X1,Z2,Y1)/ glm::length(glm::vec3(X1,Z2,Y1)));

            c.push_back(glm::vec3(1.0f, 0.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 1.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 0.0f,  1.0f));c.push_back(glm::vec3(1.0f, 0.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 1.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 0.0f,  1.0f));

        }
    }

 /*   v.push_back(glm::vec3(0.0f,  10.0f,   0.0f));
    v.push_back(glm::vec3(10.0f, -10.0f,   0.0f));
    v.push_back(glm::vec3(-10.0f, -10.0f,   0.0f));
    c.push_back(glm::vec3(1.0f, 0.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 1.0f,  0.0f));
    c.push_back(glm::vec3(0.0f, 0.0f,  1.0f));*/


    GLuint vertex_vbo;
    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, v.size() * sizeof (glm::vec3), &v[0], GL_STATIC_DRAW);
    
    GLuint colours_vbo;
    glGenBuffers (1, &colours_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
    glBufferData (GL_ARRAY_BUFFER, c.size() * sizeof (glm::vec3), &c[0], GL_STATIC_DRAW);
    
    /* create the VAO.
    we bind each VBO in turn, and call glVertexAttribPointer() to indicate where
    the memory should be fetched for vertex shader input variables 0, and 1,
    respectively. we also have to explicitly enable both 'attribute' variables.
    'attribute' is the older name for vertex shader 'in' variables. */
    size = v.size();
    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glBindBuffer (GL_ARRAY_BUFFER, colours_vbo);
    glVertexAttribPointer (1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
  


}
