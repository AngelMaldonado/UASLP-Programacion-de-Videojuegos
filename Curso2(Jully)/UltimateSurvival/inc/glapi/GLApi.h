#pragma once
/**
 * @brief This is a short API to use the functions of GLFW for rendering and manipuate
 * graphics buffers.
 * 
 * @author AngelMaldonado 
 * @file glapi.h
 * @link Github: https://github.com/AngelMaldonado
 * @date Tue Apr 11 2023
 */

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glapi/Window.h>

#define GLAPI_GLFW_VERSION 3
#define GLAPI_DEF_WINDOW_WIDTH 500
#define GLAPI_DEF_WINDOW_HEIGHT 500

#define ASSERT(f) if(!(f)) __debugbreak() // If x is false, then raise a breakpoint
#define GLCall(f) GLApi::GLValidate(); f; ASSERT(GLApi::GLLogCall(#f, __FILE__, __LINE__)) // Assertion wrapper

class GLApi {
public:
	/**
	 * Current and only window in the API
	 */
	Window window;

	/**
	 *	Prevent further constructions of GLApi
	 */
	GLApi(GLApi& other) = delete;
	
	/**
	 * Prevent assignation of GLApi instances
	 */
	void operator=(const GLApi&) = delete;

	/**
	 * Obtains the current and only instance of GLApi
	 */
	static GLApi* GetInstance(Window* window = NULL);

	/**
	 * Frees current GLApi instance
	 */
	~GLApi();

	/**
	 * Sets current API instance window and changes the context
	 */
	void SetCurrentWindow(Window window);

	/**
	 * Function to validate successfull execution of an OpenGL function call 
	 */
	static void GLValidate();
	
	/**
	 * Function to print log error if an execution of an OpenGL function wasn't successful 
	 */
	static bool GLLogCall(const char* function, const char* file, int line);

private:
	

	/**
	 * Sets and initializes the GLFW version
	 */
	void InitGLFWVersion();

	/**
	 * Initialize glad for compatibility with OpenGL-GLFW
	 */
	void GladLoad();

protected:
	static GLApi* _glApi;
	
	/**
	 * Inaccessible constructor for public construction
	 */
	GLApi(){}
};

