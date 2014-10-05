#include "simulation.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>


#define BUFFER_OFFSET(i) ((void*)(i))

Simulation::Simulation()
/*: m_gravity(0.f, -30.f)
, m_world(m_gravity)
, m_angularSpeed(glm::pi<float>() / 4)*/
{

}

float mesh_time = 0;
void Simulation::Initialize() {

    branch_shader.load();
    leaf_shader.load();
    tree = new Tree();

    /*stageBody->CreateFixture(&chain, 0.f);

    m_stage = std::make_shared<Mesh>(stageBody);

    m_player = std::make_shared<Player>(m_world);*/

    //m_standardShader = LoadShader("../shaders/phong.vert", "../shaders/phong.frag");
    //m_standardShader = LoadShader("../shaders/vert_basic.glsl", "../shaders/frag_basic.glsl");
    //t.init();
    //root_node = new Node(nullptr);
  //  root_node->generate_mesh();
//    mesh.init();

}

float rot = 0;

void Simulation::KeyHandler(int key, int scancode, int action, int mods) {
    rot += 0.1;
	/*if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
        }
	} else if (action == GLFW_RELEASE) {
    }*/
}

void Simulation::SimulationStep(float dtSeconds) {

   tree->grow();
    //printf("%f\n", dtSeconds);
    //root_node->simulate(dtSeconds);
    //mesh_time += dtSeconds;
}

/*GLuint Simulation::m_standardShader;

void setUniform( const char *name, const glm::mat4 & m){
  GLint loc = glGetUniformLocation(Simulation::m_standardShader, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void setUniform( const char *name, const glm::mat3 & m){
  GLint loc = glGetUniformLocation(Simulation::m_standardShader, name);
  glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}
*/

void Simulation::Render(int pixelWidth, int pixelHeight) {
	glViewport(0, 0, pixelWidth, pixelHeight);

	float ratio = (float)pixelWidth / (float)pixelHeight;
    float orthoHeight = 40.f;
    float top = orthoHeight / 2.f;
    float bottom = -top;
    float left = bottom * ratio;
    float right = -left;
    glm::mat4 projection = glm::perspective(
        45.0f,
        float(pixelWidth) / float(pixelHeight), 
        0.01f, 
        1000.0f
    );
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 10, 300),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    glm::mat4 model = glm::mat4(1); // identity
    model = glm::translate(model, glm::vec3(0.0f, -100.f, 0.0f));
    model = glm::rotate(model, rot, glm::vec3(0.f, 1.f, 0.f));

    branch_shader.activate();
    branch_shader.set_model(model);
    branch_shader.set_view(view);
    branch_shader.set_projection(projection);

    leaf_shader.activate();
    leaf_shader.set_model(model);
    leaf_shader.set_view(view);
    leaf_shader.set_projection(projection);


   tree->draw(branch_shader, leaf_shader);


   /* std::map<std::string, std::vector<RenderComponent>> renderobjects;
    glm::mat4 ident = glm::mat4(1);
    root_node->enqueue(renderobjects, ident);
   // root_node->enqueue(renderobjects, newmodel);

    std::vector<RenderComponent> branches = renderobjects["branch"];
   
    std::vector<glm::vec3> verts(branches.size()*2);
    auto v = begin(verts);
    for (RenderComponent& ob : branches){
        *v++ = ob.vertex[0];
        *v++ = ob.vertex[1];
     }

    GLuint vertex_vbo; 
    GLuint vao; 

    glGenBuffers (1, &vertex_vbo);
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glBufferData (GL_ARRAY_BUFFER, verts.size() * sizeof (glm::vec3), &verts[0], GL_STATIC_DRAW);

    glGenVertexArrays (1, &vao);
    glBindVertexArray (vao);
    
    glBindBuffer (GL_ARRAY_BUFFER, vertex_vbo);
    glVertexAttribPointer (0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray (0);
    glDrawArrays (GL_LINES, 0, branches.size()*2);

    glDeleteBuffers(1, &vertex_vbo);
    glDeleteBuffers(1, &vao);*/
    //DrawMesh(mesh, view, projection);
}

/*void Simulation::DrawMesh(Mesh& mesh, glm::mat4& view, glm::mat4& projection) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -12.0f, 0.0f));
   // model = glm::rotate(model, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
   // std::cout << glm::to_string(model) << std::endl;

    glm::mat4 mv = view * model;
    branch_shader.set_model_view(mv);

    glm::mat3 nm = glm::mat3( glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]) );
    branch_shader.set_normal_matrix(nm);
    
    glm::mat4 mvp = projection * mv;
    branch_shader.set_mvp(mvp);
   // setUniform("MVP", mvp);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    glEnableVertexAttribArray (2);
    glBindVertexArray (mesh.vao);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, mesh.size);
}*/

