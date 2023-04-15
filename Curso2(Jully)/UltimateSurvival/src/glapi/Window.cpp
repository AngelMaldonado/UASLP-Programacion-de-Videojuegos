#include <glapi/GLApi.h>

GLFWwindow* Window::GetGLFWwindow() { return _window; }

Window::Window(WindowSettings windowSettings)
{
	_windowSettings = windowSettings;
	_window = glfwCreateWindow(_windowSettings.width, _windowSettings.height, _windowSettings.name.c_str(), NULL, NULL);
}

void Window::Prepare()
{
	glfwSetFramebufferSizeCallback(_window, _windowSettings.windowSizeFunction);
	glfwSetCursorPosCallback(_window, _windowSettings.cursorPositionFunction);

	glfwSetScrollCallback(_window, _windowSettings.scrollFunction);
	glfwSetInputMode(_window, _windowSettings.inputMode, _windowSettings.cursorMode);
	glfwSetKeyCallback(_window, _windowSettings.keyFunction);

	if(_windowSettings.mode3D) GLCall(glEnable(GL_DEPTH_TEST));
}

void Window::SetSize(int width, int height)
{
	_windowSettings.width = width;
	_windowSettings.height = height;
    glViewport(0, 0, _windowSettings.width, _windowSettings.height);
}

void Render()
{
	//GLCall(glDrawElements(GL_TRIANGLES, GLApi::GetInstance()->))
}

bool Window::WindowIsOpen()
{
	return !glfwWindowShouldClose(_window);
}

void Window::CloseWindow()
{
	glfwSetWindowShouldClose(_window, true);
}

void defaultWindowSizeFunction(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void defaultCursorPositionFunction(GLFWwindow* window, double xpos, double ypos) { }

void defaultScrollFunction(GLFWwindow* window, double xoffset, double yoffset) { }

void defaultKeyFunction(GLFWwindow* window, int key, int scancode, int action, int mods) { }

