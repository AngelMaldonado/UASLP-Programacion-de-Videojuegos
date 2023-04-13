#include <game/Game.h>

/**
 * TODO: Be able to create a glApi instance given a defined window, the error is in the window attribute
 * of GLApi
 */

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}


Game::Game() { 
	/*Window window = Window(
		800, 800, "MyWindow", framebuffer_size_callback
	);*/
	glApi = GLApi::GetInstance(); 
}
Game::~Game() {}

void Game::Run()
{
	VertexBufferLayout bufferLayout;
	VertexArrayObject vao = VertexArrayObject(true);
	VertexBufferObject vbo = VertexBufferObject(CUBE_BUFFER, CUBE_BUFFER_SIZE);
	ElementBufferObject ebo = ElementBufferObject(CUBE_INDICES, CUBE_NUM_INDICES);

	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(3);
	bufferLayout.Push<float>(2);
	vao.AddBuffer(vbo, bufferLayout);

	glApi->window.renderer.SetBufferObjects(vao, vbo, ebo);

	while (glApi->window.WindowIsOpen())
	{
		glApi->window.renderer.Render();
		glfwSwapBuffers(glApi->window.GetGLFWwindow());
		glfwPollEvents();
	}
	glApi->~GLApi();
}

