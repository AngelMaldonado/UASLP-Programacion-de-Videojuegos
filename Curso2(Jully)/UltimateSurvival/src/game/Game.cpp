#include <game/Game.h>

Game::Game() { 
	glApi = GLApi::GetInstance(); 
}
Game::~Game() {}

void Game::Run()
{
	while (glApi->window.WindowIsOpen())
	{
		glfwSwapBuffers(glApi->window.GetGLFWwindow());
		glfwPollEvents();
	}
	glApi->~GLApi();
}

