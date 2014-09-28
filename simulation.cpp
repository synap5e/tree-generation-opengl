#include "simulation.hpp"
#include "loadShader.hpp"


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

float yrot = 0;
void Simulation::Initialize() {
    /*stageBody->CreateFixture(&chain, 0.f);

    m_stage = std::make_shared<Mesh>(stageBody);

    m_player = std::make_shared<Player>(m_world);*/

    //m_standardShader = LoadShader("../shaders/phong.vert", "../shaders/phong.frag");
    m_standardShader = LoadShader("../shaders/vert_basic.glsl", "../shaders/frag_basic.glsl");
    //t.init();

    mesh.init();
}

void Simulation::KeyHandler(int key, int scancode, int action, int mods) {
	/*if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
        }
	} else if (action == GLFW_RELEASE) {
    }*/
}

void Simulation::SimulationStep(float dtSeconds) {
   // printf("%f\n", dtSeconds);
    yrot += dtSeconds;
}

void Simulation::setUniform( const char *name, const glm::mat4 & m){
  GLint loc = glGetUniformLocation(m_standardShader, name);
  glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Simulation::setUniform( const char *name, const glm::mat3 & m){
  GLint loc = glGetUniformLocation(m_standardShader, name);
  glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

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
        0.1f, 
        100.0f
    );
    glm::mat4 view = glm::lookAt(
        glm::vec3(0, 5, 20),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0)
    );
    

    glUseProgram(m_standardShader);
    setUniform("ProjectionMatrix", projection);

    glm::mat4 i = glm::mat4(1);
    DrawMesh(mesh, view, projection);
}

void Simulation::DrawMesh(Mesh& mesh, glm::mat4& view, glm::mat4& projection) {
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(0.0f, -12.0f, 0.0f));
    model = glm::rotate(model, yrot, glm::vec3(0.0f, 1.0f, 0.0f));
   // std::cout << glm::to_string(model) << std::endl;

    glm::mat4 mv = view * model;
    setUniform("ModelViewMatrix", mv);
    setUniform("NormalMatrix", glm::mat3( glm::vec3(mv[0]), glm::vec3(mv[1]), glm::vec3(mv[2]) ));
    glm::mat4 mvp = projection * mv;
    setUniform("MVP", mvp);

    glEnableVertexAttribArray (0);
    glEnableVertexAttribArray (1);
    glEnableVertexAttribArray (2);
    glBindVertexArray (mesh.vao);
    glDrawArrays (GL_TRIANGLE_STRIP, 0, mesh.size);
}

