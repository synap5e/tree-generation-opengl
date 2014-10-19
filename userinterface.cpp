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

    glm::vec3 camera = centre + look*radius;// + glm::vec3(0, -200, 0);

    view = glm::lookAt(
        camera,
        centre,
        glm::vec3(0, 1, 0)
    );

    for (int i=0;i<9;i++){
        enable[i] = true;
    }
}

void UserInterface::key_handler(int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action==GLFW_RELEASE){
        if (key == GLFW_KEY_1){
            enable[1-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_2){
            enable[2-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_3){
            enable[3-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_4){
            enable[4-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_5){
            enable[5-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_6){
            enable[6-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_7){
            enable[7-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_8){
            enable[8-1] = action==GLFW_RELEASE;
        } else if (key == GLFW_KEY_9){
            enable[9-1] = action==GLFW_RELEASE;
        } 
    }
}

void UserInterface::mouse_drag(double x, double y){
    double ox = xrot;

    xrot -= y / 10;
    yrot -= x / 10;

    if (xrot >= 180) xrot = ox;
    if (xrot < 0) xrot = ox;

    regen_view();
}   

void UserInterface::mouse2_drag(double x, double y){
    centre.y += y/-10.f;
    regen_view();
}

void UserInterface::scroll(double ammount){
    radius = radius = fmin(500, fmax(4, radius - ammount * 10));
    regen_view();
}   
