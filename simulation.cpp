#include "simulation.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>

Simulation::Simulation(){

}

float mesh_time = 0;
void Simulation::initialize() {

    branch_shader.load();
    point_shader.load();
    tree = new Tree();

}

void Simulation::key_handler(int key, int scancode, int action, int mods) {
	/*if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
        }
	} else if (action == GLFW_RELEASE) {
    }*/
}

void Simulation::simulation_step(float dtSeconds) {
   tree->grow();
}

void Simulation::mouse_drag(double x, double y){
    double ox = xrot;

    xrot -= y*radius / 1000;
    yrot -= x*radius / 1000;

    if (xrot >= 180) xrot = ox;
    if (xrot < 0) xrot = ox;
}   

void Simulation::render(int pixelWidth, int pixelHeight) {
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

    float x_circ = cosf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    float y_circ =                            cosf(glm::radians(xrot + 180));
    float z_circ = sinf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    glm::vec3 look(x_circ, y_circ, z_circ);


    glm::vec3 centre(0, 100, 0);
    glm::vec3 camera = centre + look*radius;

    glm::mat4 view = glm::lookAt(
        camera,
        centre,
        glm::vec3(0, 1, 0)
    );
    glm::mat4 model = glm::mat4(1); // identity
  //  model = glm::translate(model, glm::vec3(0.0f, -100.f, 0.0f));
   // model = glm::rotate(model, yrot, glm::vec3(0.f, 1.f, 0.f));
   // model = glm::rotate(model, xrot, glm::vec3(-1.f, 0.f, 0.f));

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
