#include "userinterface.hpp"

#include <GLFW/glfw3.h>
#include <glm/gtc/constants.hpp>
#include <math.h>
#include <glm/gtx/string_cast.hpp>

UserInterface::UserInterface(){
    regen_view();
}

void UserInterface::regen_view(){
    float x_circ = cosf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    float y_circ =                            cosf(glm::radians(xrot + 180));
    float z_circ = sinf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    glm::vec3 look(x_circ, y_circ, z_circ);


    glm::vec3 centre(0, 0, 0);
    glm::vec3 camera = centre + look*radius;// + glm::vec3(0, -200, 0);

    view = glm::lookAt(
        camera,
        centre,
        glm::vec3(0, 1, 0)
    );
}

/*float mesh_time = 0;
void UserInterface::initialize() {

    branch_shader.load();
    point_shader.load();

    tree = new Tree();
    renderer = new TreeRenderer(tree);
}*/

void UserInterface::key_handler(int key, int scancode, int action, int mods) {
	/*if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_LEFT) {
        }
	} else if (action == GLFW_RELEASE) {
    }*/
}

/*void UserInterface::simulation_step(float dtSeconds) {
   tree->grow();
}
*/
void UserInterface::mouse_drag(double x, double y){
    double ox = xrot;

    xrot -= y*radius / 1000;
    yrot -= x*radius / 1000;

//    printf("%f, %f, %f\n", xrot, yrot, radius);

    if (xrot >= 180) xrot = ox;
    if (xrot < 0) xrot = ox;

    regen_view();
}   

void UserInterface::scroll(double ammount){
    radius = radius = fmin(500, fmax(4, radius - ammount * 10));
    regen_view();
}   


/*void UserInterface::render(int pixelWidth, int pixelHeight) {
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
    projection = glm::translate(projection, glm::vec3(0, -130, 0));

    float x_circ = cosf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    float y_circ =                            cosf(glm::radians(xrot + 180));
    float z_circ = sinf(glm::radians(yrot)) * sinf(glm::radians(xrot + 180));
    glm::vec3 look(x_circ, y_circ, z_circ);


    glm::vec3 centre(0, 0, 0);
    glm::vec3 camera = centre + look*radius;// + glm::vec3(0, -200, 0);

    glm::mat4 view = glm::lookAt(
        camera,
        centre,
        glm::vec3(0, 1, 0)
    );
    glm::mat4 model = glm::mat4(1);

    branch_shader.activate();
    branch_shader.set_model(model);
    branch_shader.set_view(view);
    branch_shader.set_projection(projection);

    point_shader.activate();
    point_shader.set_model(model);
    point_shader.set_view(view);
    point_shader.set_projection(projection);


   tree->draw(branch_shader, point_shader);


}*/
