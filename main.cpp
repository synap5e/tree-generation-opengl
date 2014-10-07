#include "glHeaders.hpp"
#include "userinterface.hpp"
#include "tree.hpp"
#include "treerenderer.hpp"
#include "random.hpp"

#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>
#include <chrono>

#include <glm/glm.hpp>

UserInterface interface;
Tree *tree;
TreeRenderer *renderer;

static void error_callback(int error, const char* description){
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else
		interface.key_handler(key, scancode, action, mods);
}


bool mousedown = false;
static void mousebutton_callback(GLFWwindow* window, int button, int actions, int mods){
	if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_PRESS){
		mousedown = true;
		//lx = -1;
	} else if (button == GLFW_MOUSE_BUTTON_1 && actions == GLFW_RELEASE){
		mousedown = false;
	}
} 

double lx;
double ly;
static void cursorpos_callback(GLFWwindow* window, double x, double y){
	if (mousedown){
		interface.mouse_drag(lx - x, ly - y);
	}

	lx = x;
	ly = y;
} 

static void scroll_callback(GLFWwindow* window, double x, double y){
	interface.scroll(y);
}

void render(int pixelWidth, int pixelHeight){
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

    renderer->render(projection, interface.view);

}

int main(void)
{
	RandomGen::seed(1337);
	RandomGen::rseed();

	//Initialize GLFW
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_SAMPLES, 4); 
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mousebutton_callback);
	glfwSetCursorPosCallback(window, cursorpos_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//glDebugMessageCallback( myCallback, NULL );

	glewExperimental = GL_TRUE;
	GLenum glewInitStatus = glewInit();
	if (glewInitStatus != GLEW_OK)
	{
		TRACE("Glew Init Error: " << glewGetErrorString(glewInitStatus));
		glfwDestroyWindow(window);
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glClearColor(0.9f, 0.9f, 0.87f, 0.0f);

	glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);

	GLenum err;
	err = glGetError();
	if(err != GL_NO_ERROR)
		printf(">> GL error: %d \n", err);


	tree = new Tree();
	renderer = new TreeRenderer(tree);

/*	std::chrono::duration<double> t(0.0);
	std::chrono::duration<double> dt(0.1);
	std::chrono::duration<double> accumulator(0.0);

	std::chrono::time_point<std::chrono::system_clock> currentTime, newTime;
	currentTime = std::chrono::system_clock::now();


	
	while (!glfwWindowShouldClose(window))
	{
		newTime = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = newTime - currentTime;
		currentTime = newTime;
		accumulator += elapsed_seconds;

		//Simulation
		while (accumulator >= dt)
		{
			g_simulation.simulation_step((float)dt.count());
			accumulator -= dt;
			t += dt;
		}*/

		//Render
	while (!glfwWindowShouldClose(window)){
		tree->grow();

		int width, height;
		glfwGetFramebufferSize(window, &width, &height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		render(width, height);
		glfwSwapBuffers(window);
		glfwPollEvents();

		GLenum err;
		err = glGetError();
		if(err != GL_NO_ERROR){
			printf("GL error: %d \n", err);
		}
	}

	//Main loop has exited, clean up
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
