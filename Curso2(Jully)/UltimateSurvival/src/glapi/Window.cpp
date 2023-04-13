#include <glapi/GLApi.h>

GLFWwindow* Window::GetGLFWwindow() { return _window; }

Window::Window() {}

Window::Window(int width, int height, std::string name, GLFWframebuffersizefun windowSizeFunction, GLFWcursorposfun cursorPositionFunction, GLFWscrollfun scrollFunction, GLFWkeyfun keyFunction, int inputMode, int value, bool mode3D)
{
	_window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
	
	this->windowSizeFunction = windowSizeFunction;
	this->cursorPositionFunction = cursorPositionFunction;
	this->scrollFunction = scrollFunction;
	this->keyFunction = keyFunction;
	this->inputMode = inputMode;
	this->cursorMode = value;
	this->mode3D = mode3D;
}

Window::~Window() {}

void Window::SetSize(int width, int height)
{
	_width = width;
	_height = height;
    glViewport(0, 0, _width, _height);
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

