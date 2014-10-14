#include "glHeaders.hpp"
#include "userinterface.hpp"
#include "tree.hpp"
#include "treerenderer.hpp"
#include "random.hpp"

#include "leafmodel.hpp"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <sstream>
#include <vector>
#include <atomic>
//#include <chrono>
#include <thread>

#include <glm/glm.hpp>

UserInterface interface;
Tree *tree;
TreeRenderer *renderer;

//float model_update_fps = 10;
std::atomic<bool> simulate;
std::atomic<bool> regenerate_display;
std::atomic<bool> updating;


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

static void run_simulation(){
	while (simulate){
		updating = true;
		if (!tree->grow()){
			updating = false;
			std::cout << "Tree growth finished\n";
			break;
		}
		updating = false;
		while (regenerate_display && simulate);
	}
	std::cout << "Simulation thread ending\n";
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

    /*if (regenerate_display){
    	renderer->regenerate();
    	regenerate_display = false;
    }*/
    renderer->render(projection, interface.view);
}


double calcFPS(GLFWwindow* window, double theTimeInterval = 1.0)
{
	// Static values which only get initialised the first time the function runs
	static double t0Value       = glfwGetTime(); // Set the initial time to now
	static int    fpsFrameCount = 0;             // Set the initial FPS frame count to 0
	static double fps           = 0.0;           // Set the initial FPS value to 0.0
 
	// Get the current time in seconds since the program started (non-static, so executed every time)
	double currentTime = glfwGetTime();
 
	// Ensure the time interval between FPS checks is sane (low cap = 0.1s, high-cap = 10.0s)
	// Negative numbers are invalid, 10 fps checks per second at most, 1 every 10 secs at least.
	if (theTimeInterval < 0.1)
	{
		theTimeInterval = 0.1;
	}
	if (theTimeInterval > 10.0)
	{
		theTimeInterval = 10.0;
	}
 
	// Calculate and display the FPS every specified time interval
	if ((currentTime - t0Value) > theTimeInterval)
	{
		// Calculate the FPS as the number of frames divided by the interval in seconds
		fps = (double)fpsFrameCount / (currentTime - t0Value);
 

		// Convert the fps value into a string using an output stringstream
		std::ostringstream stream;
		stream << fps;
		std::string fpsString = stream.str();

		// Append the FPS value to the window title details
		std::string theWindowTitle = " | FPS: " + fpsString;

		// Convert the new window title to a c_str and set it
		const char* pszConstString = theWindowTitle.c_str();
		glfwSetWindowTitle(window, pszConstString);
	
		std::cout << "FPS: " << fps << std::endl;
		
		// Reset the FPS frame counter and set the initial time to be now
		fpsFrameCount = 0;
		t0Value = glfwGetTime();
	}
	else // FPS calculation time interval hasn't elapsed yet? Simply increment the FPS frame counter
	{
		fpsFrameCount++;
	}
 
	// Return the current FPS - doesn't have to be used if you don't want it!
	return fps;
}

int main(void)
{
	RandomGen::seed(1337);
	//RandomGen::rseed();

	//Initialize GLFW
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwSwapInterval(1);
	glfwWindowHint(GLFW_SAMPLES, 4); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
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
	simulate = true;
	regenerate_display = false;
	updating = false;
	renderer->regenerate();


	std::thread simulation(run_simulation);

	double fps = 60;
	double model_fps = 15;
	double last_render_frame, last_model_frame, now, frame_delta, model_delta;
	last_render_frame = last_model_frame = glfwGetTime();
	while (!glfwWindowShouldClose(window)){
		now = glfwGetTime();
		frame_delta = now - last_render_frame;
		model_delta = now - last_model_frame;

		//std::cout << frame_delta << "\n";
		if (frame_delta < 1./fps){
			std::chrono::milliseconds dura(int(1000 * (1./fps - frame_delta)));
			std::this_thread::sleep_for( dura );
			continue;
		}

		last_render_frame = now;

		if (model_delta > 1./model_fps){
			if (regenerate_display && !updating){
				renderer->regenerate();
				regenerate_display = false;
				last_model_frame = now;
			} else {
				regenerate_display = true;
			}
			// want to do an update
			/*regenerate_display = true;
			while (updating);*/
			// update thread is now waiting for regenerate_display to go false
			
		}

		calcFPS(window);

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
	std::cout << "simulate = false\n";
	simulate = false;
	std::cout << simulate << "\n";
	simulation.join();
	std::cout << "Simulation thread ended" << std::endl;

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(EXIT_SUCCESS);
}
