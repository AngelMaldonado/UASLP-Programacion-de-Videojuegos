#pragma once
/**
 * @brief This class is used to make and manage GLFWwindow instances 
 * 
 * @author AngelMaldonado 
 * @file glapi.h
 * @link Github: https://github.com/AngelMaldonado
 * @date Tue Apr 11 2023
 */

#include <glapi/GLApi.h>

#define GLAPI_DEF_WINDOW_WIDTH 500
#define GLAPI_DEF_WINDOW_HEIGHT 500

void defaultWindowSizeFunction(GLFWwindow* window, int width, int height);
void defaultCursorPositionFunction(GLFWwindow* window, double xpos, double ypos);
void defaultScrollFunction(GLFWwindow* window, double xoffset, double yoffset);
void defaultKeyFunction(GLFWwindow* window, int key, int scancode, int action, int mods);

struct WindowSettings {
	GLFWframebuffersizefun windowSizeFunction;
	GLFWcursorposfun cursorPositionFunction;
	GLFWscrollfun scrollFunction;
	GLFWkeyfun keyFunction;
	std::string name;
	int width, height;
	int inputMode;
	int cursorMode;
	bool mode3D;
	WindowSettings() :
		windowSizeFunction(defaultWindowSizeFunction),
		cursorPositionFunction(defaultCursorPositionFunction),
		scrollFunction(defaultScrollFunction),
		keyFunction(defaultKeyFunction),
		name("GLApi Window"),
		width(GLAPI_DEF_WINDOW_WIDTH),
		height(GLAPI_DEF_WINDOW_HEIGHT),
		inputMode(GLFW_CURSOR),
		cursorMode(GLFW_CURSOR_NORMAL),
		mode3D(true) {}
};

class Window {
public:
	Renderer renderer;

	Window() {}

	Window(WindowSettings windowSettings);
	~Window() {}
	GLFWwindow* GetGLFWwindow();
	void SetSize(int width, int height);
	void Prepare();
	bool WindowIsOpen();
	void CloseWindow();

	
	GLFWframebuffersizefun GetWindowSizeFunc() const { return _windowSettings.windowSizeFunction; }
	GLFWcursorposfun GetCursorPosFunc() const { return _windowSettings.cursorPositionFunction; }
	GLFWscrollfun GetScrollFunc() const { return _windowSettings.scrollFunction; }
	GLFWkeyfun GetKeyFunc() const { return _windowSettings.keyFunction; }
private:
	WindowSettings _windowSettings;
	GLFWwindow* _window;
};

