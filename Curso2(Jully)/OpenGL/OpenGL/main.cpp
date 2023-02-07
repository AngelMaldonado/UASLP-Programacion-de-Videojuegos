#include <iostream>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace std;

const int width = 200;
const int height = 100;

void initGLFWVersion();
bool gladLoad();
void updateWindow(GLFWwindow* window);

int main() {
	initGLFWVersion();

	GLFWwindow* window = glfwCreateWindow(200, 100, "OpenGL", NULL, NULL);

	if (window == NULL) {
		cout << "No se pudo crear la ventana!" << endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoad()) return -1;

	glViewport(0, 0, width, height);

	updateWindow(window);

	glfwTerminate();

	return 0;
}

void initGLFWVersion() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

bool gladLoad() {
	bool active = true;
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		cout << "No se pudo cargar GLAD!" << endl;
		active = false;
	}

	return active;
}

void updateWindow(GLFWwindow* window) {
	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
