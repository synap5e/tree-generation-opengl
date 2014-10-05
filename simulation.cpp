#include "simulation.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>

Simulation::Simulation(){

}

float mesh_time = 0;
void Simulation::Initialize() {

    branch_shader.load();
    point_shader.load();
    tree = new Tree();

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

    point_shader.activate();
    point_shader.set_model(model);
    point_shader.set_view(view);
    point_shader.set_projection(projection);


   tree->draw(branch_shader, point_shader);


}
