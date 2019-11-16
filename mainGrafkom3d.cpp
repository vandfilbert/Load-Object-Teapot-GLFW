#include <iostream>
#include <stdlib.h>
#include <random>
#include <ctime>
#include <glew.h>
#include <glfw3.h>
#include "Shader.h"
#include "Shape.h"
#include "glm/ext.hpp"
using namespace std;

GLFWwindow* window;
GLuint VertexArrayID;
int WINDOW_WIDTH = 1000, WINDOW_HEIGHT = 1000;
Mesh mesh("asset/object/teapot.obj");

void mouseMoveEvent(GLFWwindow* window, double x, double y)
{
	double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
	double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
	printf("X : %f, Y : %f\n", mod_x, mod_y);
}
void mouseClickEvent(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		double x, y;
		glfwGetCursorPos(window, &x, &y);

		double mod_x = (float)(x - (WINDOW_WIDTH / 2)) / (float)(WINDOW_WIDTH / 2);
		double mod_y = (float)(WINDOW_HEIGHT - y - (WINDOW_HEIGHT / 2)) / (float)(WINDOW_HEIGHT / 2);
		if (action == GLFW_PRESS)
		{
			printf("LEFT CLICK ON : (%lf, %lf)\n", mod_x, mod_y);
		}
		else if (action == GLFW_RELEASE)
		{
			printf("LEFT RELEASE ON : (%lf, %lf)\n", mod_x, mod_y);
		}
	}
}
void screenResizeEvent(GLFWwindow* window, int width, int height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
}
void keyboardPressEvent(GLFWwindow* window, int key, int scancode, int action, int mods) {

}
void initializeGLFW() {
	glewExperimental = true; // Needed for core profile
	if (!glfwInit())
	{
		printf("Failed to initialize GLFW\n");
		return;
	}

	glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); // We want OpenGL 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // We don't want the old OpenGL
}
void initializeWindow() {
	// Open a window and create its OpenGL context
	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Computer Graphics", NULL, NULL);
	if (window == NULL) {
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
		glfwTerminate();
		return;
	}
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouseMoveEvent);
	glfwSetMouseButtonCallback(window, mouseClickEvent);
	glfwSetKeyCallback(window, keyboardPressEvent);
	glfwSetFramebufferSizeCallback(window, screenResizeEvent);

	glEnable(GL_DEPTH_TEST);
}
void initializeGLEW() {
	glfwMakeContextCurrent(window); // Initialize GLEW
	glewExperimental = true; // Needed in core profile
	if (glewInit() != GLEW_OK) {
		printf("Failed to initialize GLEW\n");
		return;
	}
}

void initializeShapes() {
	float fun;
	glGenVertexArrays(1, &VertexArrayID);
	glBindVertexArray(VertexArrayID);

	char vertexShader[][100] = { "asset/color/vertex.shader" };
	char fragmentShader[][100] = { "asset/color/fragment.shader" };
	char fragmentOutlineShader[][100] = { "asset/color/fragment_outline.shader" };

	mesh.Initialize_Object();
	mesh.Initialize_UVS();
	mesh.Initialize_Combination_Shading(Color(0.7f, 0.2f, 2.7f));
	mesh.initialize_shader(vertexShader[0], fragmentShader[0]);
}

void render() {
	do {
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		mesh.DrawObject();

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
		glfwWindowShouldClose(window) == 0);
}

void cleanup() {
	glDeleteVertexArrays(1, &VertexArrayID);
	glfwTerminate();
}

int main() {
	initializeGLFW();
	initializeWindow();
	initializeGLEW();
	initializeShapes();
	render();
}