#include "glHeaders.hpp"
#include "simulation.hpp"
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

Simulation g_simulation;

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
	else
		g_simulation.KeyHandler(key, scancode, action, mods);
}

int main(void)
{
	RandomGen::seed(1337);

	//Initialize GLFW
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, key_callback);

	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
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

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glEnable(GL_DEPTH_TEST);
   // glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LESS);


	g_simulation.Initialize();



	std::chrono::duration<double> t(0.0);
//	std::chrono::duration<double> dt(0.01);
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
			g_simulation.SimulationStep((float)dt.count());
			accumulator -= dt;
			t += dt;
		}

		//Render
		{
			//If user has resized window, update viewport and projection
			int width, height;
			glfwGetFramebufferSize(window, &width, &height);

			//Draw
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			g_simulation.Render(width, height);
			glfwSwapBuffers(window);
			glfwPollEvents();

			GLenum err;
			err = glGetError();
			if(err != GL_NO_ERROR)
				printf("GL error: %d \n", err);

		}
	}

	//Main loop has exited, clean up
	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
