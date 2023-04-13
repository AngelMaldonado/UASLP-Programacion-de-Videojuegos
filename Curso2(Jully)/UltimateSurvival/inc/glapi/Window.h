#pragma once
/**
 * @brief This class is used to make and manage GLFWwindow instances 
 * 
 * @author AngelMaldonado 
 * @file glapi.h
 * @link Github: https://github.com/AngelMaldonado
 * @date Tue Apr 11 2023
 */

#include <GLFW/glfw3.h>
#include <iostream>

class Window {
public:
	GLFWframebuffersizefun windowSizeFunction;
	GLFWcursorposfun cursorPositionFunction;
	GLFWscrollfun scrollFunction;
	GLFWkeyfun keyFunction;
	int inputMode;
	int cursorMode;
	bool mode3D;
	Renderer renderer;

	Window();
	Window(int width, int height, std::string name, GLFWframebuffersizefun windowSizeFunction = NULL, GLFWcursorposfun cursorPositionFunction = NULL, GLFWscrollfun scrollFunction = NULL, GLFWkeyfun keyFunction = NULL, int inputMode = GLFW_CURSOR, int value = GLFW_CURSOR_DISABLED, bool mode3D = true);
	~Window();
	GLFWwindow* GetGLFWwindow();
	void SetSize(int width, int height);
	bool WindowIsOpen();
	void CloseWindow();
private:
	int _width, _height;
	std::string _name;
	GLFWwindow* _window;
};

